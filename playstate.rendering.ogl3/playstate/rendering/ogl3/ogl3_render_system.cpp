#include <playstate/memory/memory.h>
#include <playstate/game/configuration.h>
#include "ogl3_render_system.h"
using namespace playstate;

namespace {
	GLuint _current_frameBufferObject = 0;
	RenderTarget2D* _current_depthRenderTarget = NULL;
	RenderTarget2D* _current_renderTargets[MaxDrawBuffers] = {NULL};
}

OGL3RenderSystem::OGL3RenderSystem(IWindow& window, ScriptSystem& scriptSystem) 
	: IRenderSystem(),
	mWindow(window), mProgramFactory(NULL), 
	mUniformVertexBuffer(NULL), 
	mFrameBufferId(0), mDepthRenderTarget(NULL), mGfxPrograms(offsetof(GfxProgram, Link))
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
	mProgramFactory = new GfxProgramFactory(*this, scriptSystem);
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

GfxProgram* OGL3RenderSystem::LoadGfxProgram(const playstate::string& fileName)
{
	GfxProgram* program = NULL;
	try {
		program = mProgramFactory->Create(fileName);
	} catch(Exception& e) {
		bool developmentMode = IConfiguration::Get().FindBool("graphics.developmentmode", true);
		if(developmentMode) {
			ILogger::Get().Error("Could not compile graphics program: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
			program = new GfxProgram(*this);
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
	StatePolicyGuard::MarkAsDirty();

	// Mark gfx programs as dirty
	GfxProgram* program = mGfxPrograms.First();
	while(program != NULL) {
		program->MarkAsDirty();
		program = program->Link.Tail;
	}
}

void OGL3RenderSystem::SetRenderTarget(RenderTarget2D* renderTarget, GLenum attachmentIndex)
{
	assert(attachmentIndex < MaxDrawBuffers && "You are not allowed to attach to many render targets");
	mRenderTargets[attachmentIndex] = renderTarget;
}

void OGL3RenderSystem::SetDepthRenderTarget(RenderTarget2D* renderTarget)
{
	mDepthRenderTarget = renderTarget;
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

	GLsizei width = 0;
	GLsizei height = 0;

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
		RenderTarget2D* rt = mRenderTargets[i];
		if(rt != NULL) {
			width = rt->GetWidth();
			height = rt->GetHeight();
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
	
	StatePolicyGuard::Viewport(Rect(0, 0, width, height));
}

const Version& OGL3RenderSystem::GetShaderVersion() const
{
	return mShaderVersion;
}

VertexBuffer* OGL3RenderSystem::GetUniformVertexBuffer() const
{
	return mUniformVertexBuffer;
}

IndexBuffer* OGL3RenderSystem::CreateStaticBuffer(const uint32* indices, uint32 numIndices)
{
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_STATIC_DRAW);
	
	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create index buffer. Reason: %d", status);
	}

	return new IndexBuffer(indexBuffer, numIndices);
}


VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionData), PositionDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionTexCoordData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTexCoordData), PositionTexCoordDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionTexCoordColorData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionTexCoordColorData), PositionTexCoordColorDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionNormalData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionNormalData), PositionNormalDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionColorData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionColorData), PositionColorDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const PositionNormalTextureData* vertices, uint32 numVertices)
{
	return CreateStaticBuffer(vertices, sizeof(PositionNormalTextureData), PositionNormalTextureDataVAOFactory, numVertices);
}

VertexBuffer* OGL3RenderSystem::CreateStaticBuffer(const void* vertices, uint32 vertexSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numVertices)
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

	return new VertexBuffer(GL_TRIANGLES, arrayFactory, bufferID, numVertices, vertexSize);
}

VertexBuffer* OGL3RenderSystem::CreateDynamicBuffer(const void* vertices, uint32 vertexSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numVertices)
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

	return new VertexBuffer(GL_TRIANGLES, arrayFactory, bufferID, numVertices, vertexSize);
}

RenderTarget2D* OGL3RenderSystem::CreateRenderTarget2D(uint32 width, uint32 height, TextureFormat::Enum format)
{
	assert(width > 0.0f && "You cannot create a render target with 0 width");
	assert(height > 0.0f && "You cannot create a render target with 0 height");

	GLint _format = GL_RGBA;
	GLint _internalFormat = GL_RGBA;
	GLenum _minMag = GL_LINEAR;

	switch(format)
	{
	case TextureFormat::RGB:
		_format = GL_RGB;
		_internalFormat = GL_RGB;
		break;
	case TextureFormat::RGBA:
		break;
	case TextureFormat::RGBA8:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA8;
		break;
	case TextureFormat::RGBA12:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA12;
		break;
	case TextureFormat::RGBA16:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA16;
		break;
	case TextureFormat::RGB10_A2:
		_format = GL_RGBA;
		_internalFormat = GL_RGB10_A2;
		break;
	case TextureFormat::RGBA16F:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA16F;
		break;
	case TextureFormat::DEPTH24:
		_format = GL_DEPTH_COMPONENT;
		_internalFormat = GL_DEPTH_COMPONENT24;
		_minMag = GL_NEAREST;
		break;
	case TextureFormat::DEPTH24_STENCIL8:
		_format = GL_DEPTH_STENCIL;
		_internalFormat = GL_DEPTH24_STENCIL8;
		_minMag = GL_NEAREST;
		break;
	}

	GLuint boundTextureId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundTextureId);

	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, width, height, 0, _format, GL_FLOAT, NULL);
	glFlush();

	if(boundTextureId != 0)
		glBindTexture(GL_TEXTURE_2D, boundTextureId);

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create 2D render target. Reason: %d", status);
	}

	return new RenderTarget2D(textureId, width, height, format);
}
