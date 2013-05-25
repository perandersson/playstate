#include "../memory/memory.h"
#include "render_target_2d.h"
using namespace playstate;

RenderTarget2D::RenderTarget2D(GLuint textureId, uint32 width, uint32 height, TextureFormat format) 
	: Texture2D(textureId, width, height, format)
{
}

RenderTarget2D::~RenderTarget2D()
{
}

void RenderTarget2D::BindToFrameBuffer(GLenum drawBuffer)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, TextureID, 0);
}
