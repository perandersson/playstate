#include "../memory/memory.h"
#include "render_target_2d.h"
using namespace playstate;

RenderTarget2D::RenderTarget2D(GLuint textureId, uint32 width, uint32 height, TextureFormat::Enum format) 
	: Texture2D(textureId, width, height, format)
{
}

RenderTarget2D::~RenderTarget2D()
{
}

RenderTarget2D* RenderTarget2D::Create(uint32 width, uint32 height, TextureFormat::Enum format)
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

void RenderTarget2D::BindToFrameBuffer(GLenum drawBuffer)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, mTextureId, 0);
}
