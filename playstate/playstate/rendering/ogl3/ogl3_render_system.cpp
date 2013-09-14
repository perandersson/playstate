#include "../../memory/memory.h"
#include "../../game/configuration.h"
#include "ogl3_render_system.h"
#include "ogl3_gfx_program.h"
#include "ogl3_vertex_buffer.h"
#include "ogl3_index_buffer.h"
#include "ogl3_state_policy.h"
#include "ogl3_render_target_2d.h"
using namespace playstate;

namespace {
	GLuint _current_frameBufferObject = 0;
	OGL3RenderTarget2D* _current_depthRenderTarget = NULL;
	OGL3RenderTarget2D* _current_renderTargets[MaxDrawBuffers] = {NULL};
}

OGL3RenderSystem::OGL3RenderSystem(IWindow& window, ScriptSystem& scriptSystem) 
	: IRenderSystem(),
	mWindow(window), mProgramFactory(NULL), mVAOFactory(NULL),
	mUniformVertexBuffer(NULL), 
	mFrameBufferId(0), mDepthRenderTarget(NULL), mGfxPrograms(offsetof(OGL3GfxProgram, Link))
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
	mProgramFactory = new GfxProgramFactory(*this, scriptSystem);
	mVAOFactory = new VertexDeclarationArrayObjectFactory();
	mWindow.AddWindowResizedListener(this);

	mVersion = getVersion(GL_VERSION);
	mShaderVersion = getVersion(GL_SHADING_LANGUAGE_VERSION);

	if(!mVersion.HigherOrEqual(Version(3, 3)))
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ZERO);

	glActiveTexture(GL_TEXTURE0);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glGenFramebuffers(1, &mFrameBufferId);

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Initialization failed");

	PositionTexCoordData elements[6];
	elements[0].Position.Set(-1.0f, 1.0f, 0.0f);
	elements[0].TexCoord.Set(0.0f, 1.0f);

	elements[1].Position.Set(1.0f, 1.0f, 0.0f);
	elements[1].TexCoord.Set(1.0f, 1.0f);

	elements[2].Position.Set(-1.0f, -1.0f, 0.0f);
	elements[2].TexCoord.Set(0.0f, 0.0f);

	elements[3].Position.Set(-1.0f, -1.0f, 0.0f);
	elements[3].TexCoord.Set(0.0f, 0.0f);
	
	elements[4].Position.Set(1.0f, 1.0f, 0.0f);
	elements[4].TexCoord.Set(1.0f, 1.0f);

	elements[5].Position.Set(1.0f, -1.0f, 0.0f);
	elements[5].TexCoord.Set(1.0f, 0.0f);

	mUniformVertexBuffer = CreateStaticBuffer(elements, 6);
}

OGL3RenderSystem::~OGL3RenderSystem()
{
	mWindow.RemoveWindowResizedListener(this);

	if(mFrameBufferId != 0) {
		glDeleteFramebuffers(1, &mFrameBufferId);
		mFrameBufferId = 0;
	}

	if(mUniformVertexBuffer != NULL) {
		delete mUniformVertexBuffer;
		mUniformVertexBuffer = NULL;
	}
	
	if(mVAOFactory != NULL) {
		delete mVAOFactory;
		mVAOFactory = NULL;
	}

	delete mProgramFactory; mProgramFactory = NULL;
	mGfxPrograms.DeleteAll();
}

Version OGL3RenderSystem::getVersion(GLenum name) const
{
	int major = 0;
	int minor = 0;
	const GLubyte* str = glGetString(name);
	sscanf((const char*)str, "%d.%d", &major, &minor);
	return Version(major, minor);
}

IGfxProgram* OGL3RenderSystem::LoadGfxProgram(const playstate::string& fileName)
{
	OGL3GfxProgram* program = NULL;
	try {
		program = mProgramFactory->Create(fileName);
	} catch(Exception& e) {
		bool developmentMode = IConfiguration::Get().FindBool("graphics.developmentmode", true);
		if(developmentMode) {
			ILogger::Get().Error("Could not compile graphics program: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
			program = new OGL3GfxProgram(*this);
		} else {
			throw e;
		}
	}
	
	mGfxPrograms.AddLast(program);
	return program;
}

void OGL3RenderSystem::OnWindowResized(const Point& newSize)
{
	mScreenViewport.Width = newSize.X;
	mScreenViewport.Height = newSize.Y;

	// Mark system as dirty
	OGL3StatePolicyGuard::MarkAsDirty();

	// Mark gfx programs as dirty
	OGL3GfxProgram* program = mGfxPrograms.First();
	while(program != NULL) {
		program->MarkAsDirty();
		program = program->Link.Tail;
	}
}

void OGL3RenderSystem::SetRenderTarget(IRenderTarget2D* renderTarget, GLenum attachmentIndex)
{
	assert(attachmentIndex < MaxDrawBuffers && "You are not allowed to attach to many render targets");
	mRenderTargets[attachmentIndex] = static_cast<OGL3RenderTarget2D*>(renderTarget);
}

void OGL3RenderSystem::SetDepthRenderTarget(IRenderTarget2D* renderTarget)
{
	mDepthRenderTarget = static_cast<OGL3RenderTarget2D*>(renderTarget);
}

void OGL3RenderSystem::ApplyRenderTargets()
{
	bool disableFBO = mDepthRenderTarget == NULL;
	if(disableFBO) {
		for(int i = 0; i < MaxDrawBuffers; ++i) {
			if(mRenderTargets[i] != NULL) {
				disableFBO = false;
				break;
			}
		}
	}

	if(disableFBO) {
		if(_current_frameBufferObject != 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			StatePolicyGuard::Viewport(mScreenViewport);
			_current_frameBufferObject = 0;
		}
		
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
			THROW_EXCEPTION(RenderingException, "Could not disable bound frame buffer object. Reason: %d", error);

		return;
	}

	if(_current_frameBufferObject == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferId);
		_current_frameBufferObject = mFrameBufferId;
	}

	Size size;

	if(mDepthRenderTarget != NULL) {
		if(_current_depthRenderTarget != mDepthRenderTarget) {
			_current_depthRenderTarget = mDepthRenderTarget;
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			if(mDepthRenderTarget->GetFormat() == TextureFormat::DEPTH24_STENCIL8)
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
			mDepthRenderTarget->BindToFrameBuffer(attachmentType);
		}
	} else {
		if(_current_depthRenderTarget != NULL) {
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			if(_current_depthRenderTarget->GetFormat() == TextureFormat::DEPTH24_STENCIL8)
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
			_current_depthRenderTarget = 0;
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0);
		}
	}

	GLenum drawBuffers[MaxDrawBuffers] = {0};
	uint32 numDrawBuffers = 0;
	for(int i = 0; i < MaxDrawBuffers; ++i) {
		OGL3RenderTarget2D* rt = mRenderTargets[i];
		if(rt != NULL) {
			size = rt->GetSize();
			drawBuffers[numDrawBuffers++] = GL_COLOR_ATTACHMENT0 + i;
			if(_current_renderTargets[i] != rt) {
				_current_renderTargets[i] = rt;
				rt->BindToFrameBuffer(GL_COLOR_ATTACHMENT0 + i);
			}
		} else {
			if(_current_renderTargets[i] != 0) {
				_current_renderTargets[i] = 0;
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
			}
		}
	}
	glDrawBuffers(numDrawBuffers, drawBuffers);

	// Check for any GL errors
	GLenum status = glGetError();
	if(status != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not apply render targets. Reason: %d", status);

	// Check if all worked fine and unbind the FBO
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Attachment is NOT complete.");
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: No image is attached to FBO.");
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Attached images have different dimensions");
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Color attached images have different internal formats");
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Draw buffer");
        break;
 
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		THROW_EXCEPTION(RenderingException, "Framebuffer incomplete: Read buffer");
        break;
 
    case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		THROW_EXCEPTION(RenderingException, "Unsupported by FBO implementation");
        break;
 
    default:
		THROW_EXCEPTION(RenderingException, "Unknow error");
        break;
    }
	
	StatePolicyGuard::Viewport(Rect(0, 0, size.X, size.Y));
}

const Version& OGL3RenderSystem::GetShaderVersion() const
{
	return mShaderVersion;
}

IVertexBuffer* OGL3RenderSystem::GetUniformVertexBuffer() const
{
	return mUniformVertexBuffer;
}

IIndexBuffer* OGL3RenderSystem::CreateStaticBuffer(const uint32* indices, uint32 numIndices)
{
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_STATIC_DRAW);
	
	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create index buffer. Reason: %d", status);
	}

	return new OGL3IndexBuffer(indexBuffer, numIndices);
}


IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionData), PositionDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionTexCoordData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTexCoordData), PositionTexCoordDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionTexCoordColorData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTexCoordColorData), PositionTexCoordColorDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionNormalData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionNormalData), PositionNormalDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionColorData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionColorData), PositionColorDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionNormalTextureData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionNormalTextureData), PositionNormalTextureDataVertexDeclaration, numVertices);
}

IVertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices)
{
	assert(numVertices > 0 && "There is no point in creating a static buffer with 0 elements");

	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, vertices, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new OGL3VertexBuffer(PrimitiveType::Triangle, vertexDeclaration, bufferID, numVertices, vertexSize);
}

IVertexBuffer* OGL3RenderSystem::CreateDynamicBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	if(numVertices > 0) {
		glBufferData(GL_ARRAY_BUFFER, numVertices * vertexSize, vertices, GL_DYNAMIC_DRAW);
		glFlush();
	}

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new OGL3VertexBuffer(PrimitiveType::Triangle, vertexDeclaration, bufferID, numVertices, vertexSize);
}

GLenum OGL3RenderSystem::GetFormat(TextureFormat::Enum format)
{
	GLint _format = GL_RGBA;
	switch(format)
	{
	case TextureFormat::RGB:
		_format = GL_RGB;
		break;
	case TextureFormat::RGBA:
		break;
	case TextureFormat::RGBA8:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA12:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA16:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGB10_A2:
		_format = GL_RGBA;
		break;
	case TextureFormat::RGBA16F:
		_format = GL_RGBA;
		break;
	case TextureFormat::DEPTH24:
		_format = GL_DEPTH_COMPONENT;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_format = GL_DEPTH_STENCIL;
		break;
	case TextureFormat::R:
		_format = GL_RED;
		break;
	case TextureFormat::BGR:
		_format = GL_BGR;
		break;
	case TextureFormat::BGRA:
		_format = GL_BGRA;
		break;
	}
	return _format;
}

GLenum OGL3RenderSystem::GetInternalFormat(TextureFormat::Enum format)
{
	GLint _internalFormat = GL_RGBA;
	switch(format)
	{
	case TextureFormat::RGB:
		_internalFormat = GL_RGB;
		break;
	case TextureFormat::RGBA:
		break;
	case TextureFormat::RGBA8:
		_internalFormat = GL_RGBA8;
		break;
	case TextureFormat::RGBA12:
		_internalFormat = GL_RGBA12;
		break;
	case TextureFormat::RGBA16:
		_internalFormat = GL_RGBA16;
		break;
	case TextureFormat::RGB10_A2:
		_internalFormat = GL_RGB10_A2;
		break;
	case TextureFormat::RGBA16F:
		_internalFormat = GL_RGBA16F;
		break;
	case TextureFormat::DEPTH24:
		_internalFormat = GL_DEPTH_COMPONENT24;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_internalFormat = GL_DEPTH24_STENCIL8;
		break;
	case TextureFormat::R:
		_internalFormat = GL_R8;
		break;
	case TextureFormat::BGR:
		_internalFormat = GL_RGB;
		break;
	case TextureFormat::BGRA:
		_internalFormat = GL_RGBA;
		break;
	}
	return _internalFormat;
}

IRenderTarget2D* OGL3RenderSystem::CreateRenderTarget2D(const Size& size, TextureFormat::Enum format)
{
	assert(size.X > 0.0f && "You cannot create a render target with 0 width");
	assert(size.Y > 0.0f && "You cannot create a render target with 0 height");

	GLenum _minMag = GL_LINEAR;
	const GLenum _format = GetFormat(format);
	const GLenum _internalFormat = GetInternalFormat(format);

	switch(format)
	{
	case TextureFormat::DEPTH24:
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_minMag = GL_NEAREST;
		break;
	}

	GLuint boundTextureId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundTextureId);

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.X, size.Y, 0, _format, GL_FLOAT, NULL);
	glFlush();

	if(boundTextureId != 0)
		glBindTexture(GL_TEXTURE_2D, boundTextureId);

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D render target. Reason: %d", status);
	}

	return new OGL3RenderTarget2D(textureId, size, format);
}

ITexture2D* OGL3RenderSystem::CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes)
{
	const GLenum _format = GetFormat(format);
	const GLenum _internalFormat = GetInternalFormat(format);

	GLuint textureID = 0;	
	glGenTextures(1, &textureID);
	StatePolicyGuard::BindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, size.Width, size.Height, 0, _format, GL_UNSIGNED_BYTE, bytes);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D texture. Reason: %d", status);
	}

	OGL3Texture2D* texture = new OGL3Texture2D(textureID, size, format);
	return texture;
}
