#pragma once
#include "../render_target_2d.h"
#include "ogl3_texture2d.h"

namespace playstate
{
	class OGL3RenderTarget2D : public IRenderTarget2D
	{
	public:
		OGL3RenderTarget2D(GLuint textureId, const Size& size, TextureFormat::Enum format);
		virtual ~OGL3RenderTarget2D();

		//
		// Binds this render target to the supplied draw buffer. 
		// @param drawBuffer
		void BindToFrameBuffer(GLenum drawBuffer);

		//
		// @return The actual texture resource for this render target.
		inline OGL3Texture2D* GetTexture() {
			return &mTexture;
		}

	// ITexture2D
	public:
		virtual const Size& GetSize() const;

	// ITexture
	public:
		virtual TextureFormat::Enum GetFormat() const;

	private:
		OGL3Texture2D mTexture;		
	};
}
