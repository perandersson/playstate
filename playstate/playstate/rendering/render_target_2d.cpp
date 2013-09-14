#include "../memory/memory.h"
#include "render_target_2d.h"
#include "render_system.h"
using namespace playstate;

RenderTarget2D::RenderTarget2D(GLuint textureId, const Size& size, TextureFormat::Enum format) 
	: OGL3Texture2D(textureId, size, format)
{
}

RenderTarget2D::~RenderTarget2D()
{
}

void RenderTarget2D::BindToFrameBuffer(GLenum drawBuffer)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, mTextureID, 0);
}
