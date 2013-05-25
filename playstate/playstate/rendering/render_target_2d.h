#pragma once

#include "../types.h"
#include "texture2d.h"

namespace playstate
{
	class RenderSystem;

	//
	// Interested into drawing into a texture/bitmap instead of onto the screen? Then this class is for you
	class RenderTarget2D : public Texture2D
	{
		friend class RenderSystem;

	public:
		RenderTarget2D(GLuint textureId, uint32 width, uint32 height, TextureFormat format);
		virtual ~RenderTarget2D();
		
	private:
		void BindToFrameBuffer(GLenum drawBuffer);
	};
}
