#include "../memory/memory.h"
#include "render_system.h"
#include "factories/vertex_array_object_factories.h"
using namespace playstate;

template<> playstate::RenderSystem* playstate::Singleton<playstate::RenderSystem>::gSingleton = NULL;

namespace {
	GLuint _current_frameBufferObject = 0;
	uint32 _current_depthRenderTarget = 0;
	uint32 _current_renderTargets[MaxDrawBuffers] = {0};
}

RenderSystem::RenderSystem(IWindow& window, ScriptSystem& scriptSystem) : mWindow(window), mProgramFactory(NULL), 
	mUniformVertexBuffer(NULL), 
	mFrameBufferId(0), mDepthRenderTarget(NULL), mScreenWidth(window.GetWidth()), mScreenHeight(window.GetHeight()),
	mPositionVAOFactory(new PositionVAOFactory()), mPositionNormalVAOFactory(new PositionNormalVAOFactory()),
	mPositionTextureVAOFactory(new PositionTexCoordVAOFactory()), mPositionNormalTextureVAOFactory(new PositionNormalTextureVAOFactory()),
	UniformVertexBuffer(mUniformVertexBuffer), ShaderVersion(mShaderVersion)
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
	
	//glShadeModel(GL_SMOOTH); // Deprecated > 3.0
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);

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

	mUniformVertexBuffer = CreateStaticBufferObject(elements, 6);
	
	GLenum status = glGetError();
	if(status != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not create uniform vertex buffer buffer. Reason: %d", status);
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

	delete mPositionVAOFactory; mPositionVAOFactory = NULL;
	delete mPositionNormalVAOFactory; mPositionNormalVAOFactory = NULL;
	delete mPositionTextureVAOFactory; mPositionTextureVAOFactory = NULL;
	delete mPositionNormalTextureVAOFactory; mPositionNormalTextureVAOFactory = NULL;
	delete mProgramFactory; mProgramFactory = NULL;
}

bool RenderSystem::IsValidVersion() const
{
	const GLubyte* str = glGetString(GL_VERSION);
	double version = 0.0;
	sscanf((const char*)str, "%lf", &version);
	return version >= 3.3; 
}

IndexBuffer* RenderSystem::CreateIndexBuffer(uint32* indices, uint32 numIndices)
{
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create index buffer. Reason: %d", status);
	}

	return new IndexBuffer(indexBuffer, numIndices);
}

VertexBuffer* RenderSystem::CreateStaticBufferObject(PositionData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionData), elements, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Deprecated > 3.0
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, mPositionVAOFactory, bufferID, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBufferObject(PositionTexCoordData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionTexCoordData), elements, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Deprecated > 3.0
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, mPositionTextureVAOFactory, bufferID, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBufferObject(PositionNormalData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionNormalData), elements, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Deprecated > 3.0
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, mPositionNormalVAOFactory, bufferID, numElements);
}

VertexBuffer* RenderSystem::CreateStaticBufferObject(PositionNormalTextureData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionNormalTextureData), elements, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Deprecated > 3.0
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, mPositionNormalTextureVAOFactory, bufferID, numElements);
}

GfxProgram* RenderSystem::LoadGfxProgram(const std::string& fileName)
{
	return mProgramFactory->Create(fileName);
}

RenderTarget2D* RenderSystem::CreateRenderTarget2D(uint32 width, uint32 height, TextureFormat format)
{	
	assert(width > 0.0f && "You cannot create a render target with 0 width");
	assert(height > 0.0f && "You cannot create a render target with 0 height");

	GLint _format = GL_RGBA;
	GLint _internalFormat = GL_RGBA;
	GLenum _minMag = GL_LINEAR;

	switch(format)
	{
	case RGB:
		_format = GL_RGB;
		_internalFormat = GL_RGB;
		break;
	case RGBA:
		break;
	case RGBA8:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA8;
		break;
	case RGBA12:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA12;
		break;
	case RGBA16:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA16;
		break;
	case RGB10_A2:
		_format = GL_RGBA;
		_internalFormat = GL_RGB10_A2;
		break;
	case RGBA16F:
		_format = GL_RGBA;
		_internalFormat = GL_RGBA16F;
		break;
	case DEPTH24:
		_format = GL_DEPTH_COMPONENT;
		_internalFormat = GL_DEPTH_COMPONENT24;
		_minMag = GL_NEAREST;
		break;
	case DEPTH24_STENCIL8:
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


void RenderSystem::OnWindowResized(IWindow& window, uint32 width, uint32 height)
{
	mScreenWidth = width;
	mScreenHeight = height;
	glViewport(0, 0, mScreenWidth, mScreenHeight);

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
			glViewport(0, 0, mScreenWidth, mScreenHeight);
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
		if(_current_depthRenderTarget != mDepthRenderTarget->StateID) {
			_current_depthRenderTarget = mDepthRenderTarget->StateID;
			GLenum attachmentType = GL_DEPTH_ATTACHMENT;
			if(mDepthRenderTarget->Format == DEPTH24_STENCIL8)
				attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
			mDepthRenderTarget->BindToFrameBuffer(attachmentType);
		}
	} else {
		if(_current_depthRenderTarget != NULL) {
			GLenum attachmentType = GL_DEPTH_ATTACHMENT | GL_DEPTH_STENCIL_ATTACHMENT;
			_current_depthRenderTarget = 0;
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0);
		}
	}

	GLenum drawBuffers[MaxDrawBuffers] = {0};
	uint32 numDrawBuffers = 0;
	for(int i = 0; i < MaxDrawBuffers; ++i) {
		RenderTarget2D* rt = mRenderTargets[i];
		if(rt != NULL) {
			width = rt->Width;
			height = rt->Height;
			drawBuffers[numDrawBuffers++] = GL_COLOR_ATTACHMENT0 + i;
			if(_current_renderTargets[i] != rt->StateID) {
				_current_renderTargets[i] = rt->StateID;
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

	glViewport(0, 0, width, height);

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
