#include "../memory/memory.h"
#include "../game/configuration.h"
#include "render_system.h"
using namespace playstate;

template<> playstate::RenderSystem* playstate::Singleton<playstate::RenderSystem>::gSingleton = NULL;

namespace {
	GLuint _current_frameBufferObject = 0;
	RenderTarget2D* _current_depthRenderTarget = NULL;
	RenderTarget2D* _current_renderTargets[MaxDrawBuffers] = {NULL};
}

RenderSystem::RenderSystem(IWindow& window, ScriptSystem& scriptSystem) : mWindow(window), mProgramFactory(NULL), 
	mUniformVertexBuffer(NULL), 
	mFrameBufferId(0), mDepthRenderTarget(NULL), mScreenWidth(window.GetWidth()), mScreenHeight(window.GetHeight())
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
	mWindow.AddWindowResizedListener(this);
	mProgramFactory = new GfxProgramFactory(*this, scriptSystem);

	if(!IsValidVersion())
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");

	const GLubyte* str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	int major = 0;
	int minor = 0;
	sscanf((const char*)str, "%d.%d", &major, &minor);
	mShaderVersion = Version(major, minor);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

RenderSystem::~RenderSystem()
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

bool RenderSystem::IsValidVersion() const
{
	const GLubyte* str = glGetString(GL_VERSION);
	double version = 0.0;
	sscanf((const char*)str, "%lf", &version);
	return version >= 3.3; 
}

GfxProgram* RenderSystem::LoadGfxProgram(const std::string& fileName)
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

void RenderSystem::OnWindowResized(uint32 width, uint32 height)
{
	mScreenWidth = width;
	mScreenHeight = height;

	// Mark system as dirty
	StatePolicy::MarkAsDirty();

	// Mark gfx programs as dirty
	GfxProgram* program = mGfxPrograms.First();
	while(program != NULL) {
		program->MarkAsDirty();
		program = program->Link.Tail;
	}

	CHECK_GL_ERROR();
}

void RenderSystem::SetRenderTarget(RenderTarget2D* renderTarget, GLenum attachmentIndex)
{
	assert(attachmentIndex < MaxDrawBuffers && "You are not allowed to attach to many render targets");
	mRenderTargets[attachmentIndex] = renderTarget;
}

void RenderSystem::SetDepthRenderTarget(RenderTarget2D* renderTarget)
{
	mDepthRenderTarget = renderTarget;
}

void RenderSystem::ApplyRenderTargets()
{
	CHECK_GL_ERROR();

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
			StatePolicy::Viewport(0, 0, mScreenWidth, mScreenHeight);
			_current_frameBufferObject = 0;
		}
		
		CHECK_GL_ERROR();
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

	StatePolicy::Viewport(0, 0, width, height);

	// Check for any GL errors
	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not apply render targets. Reason: %d", status);
	}

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
}

const Version& RenderSystem::GetShaderVersion() const
{
	return mShaderVersion;
}

VertexBuffer* RenderSystem::GetUniformVertexBuffer() const
{
	return mUniformVertexBuffer;
}

IndexBuffer* RenderSystem::CreateStaticBuffer(const uint32* indices, uint32 numIndices)
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


VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionData), PositionDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionTexCoordData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionTexCoordData), PositionTexCoordDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionTexCoordColorData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionTexCoordColorData), PositionTexCoordColorDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionNormalData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionNormalData), PositionNormalDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionColorData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionColorData), PositionColorDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const PositionNormalTextureData* elements, uint32 numElements)
{
	return CreateStaticBuffer(elements, sizeof(PositionNormalTextureData), PositionNormalTextureDataVAOFactory, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBuffer(const void* data, uint32 dataTypeSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * dataTypeSize, data, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, arrayFactory, bufferID, numElements);
}

RenderTarget2D* RenderSystem::CreateRenderTarget2D(uint32 width, uint32 height, TextureFormat::Enum format)
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
