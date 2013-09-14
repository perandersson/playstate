#include "../../memory/memory.h"
#include "ogl3_render_target_2d.h"
using namespace playstate;

OGL3RenderTarget2D::OGL3RenderTarget2D(GLuint textureId, const Size& size, TextureFormat::Enum format) 
	: mTexture(textureId, size, format)
{
}

OGL3RenderTarget2D::~OGL3RenderTarget2D()
{
}

void OGL3RenderTarget2D::BindToFrameBuffer(GLenum drawBuffer)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, mTexture.GetID(), 0);
}

const Size& OGL3RenderTarget2D::GetSize() const
{
	return mTexture.GetSize();
}

TextureFormat::Enum OGL3RenderTarget2D::GetFormat() const
{
	return mTexture.GetFormat();
}
