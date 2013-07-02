#pragma once

#include "../types.h"
#include "texture2d.h"
#include "exception/rendering_exception.h"

namespace playstate
{
	class RenderSystem;

	//
	// Interested into drawing into a texture/bitmap instead of onto the screen? Then this class is for you
	class RenderTarget2D : public Texture2D
	{
		friend class RenderSystem;

	protected:
		RenderTarget2D(GLuint textureId, uint32 width, uint32 height, TextureFormat::Enum format);

	public:
		virtual ~RenderTarget2D();
		
		//
		// Creates a new render target based on the supplied width, height and format.
		// @param width The render targets width
		// @param height The render targets height
		// @param format What types of components should be in use for this render target
		// @throws RenderingException If an error occured while creating the render target
		static RenderTarget2D* Create(uint32 width, uint32 height, TextureFormat::Enum format);
		
	private:
		void BindToFrameBuffer(GLenum drawBuffer);
	};
}
