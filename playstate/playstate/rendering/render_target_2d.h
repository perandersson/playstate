#pragma once

#include "../types.h"
#include "texture2d.h"

namespace playstate
{

	//
	// Interested into drawing into a texture/bitmap instead of onto the screen? Then this class is for you
	class RenderTarget2D : public Texture2D
	{
	public:
		RenderTarget2D(GLuint textureId, uint32 width, uint32 height, TextureFormat::Enum format);
		virtual ~RenderTarget2D();
		//
		// Binds this render target to the supplied draw buffer. 
		// @param drawBuffer
		void BindToFrameBuffer(GLenum drawBuffer);
		
	private:
	};
}
