#pragma once
#include <gl/glew.h>

#include "../types.h"
#include "../resources/resource.h"
#include "../uuid.h"

namespace playstate
{
	class TextureFormat
	{
	public:
		enum Enum {
			RGB, 
			RGBA,
			RGBA8,
			RGBA12,
			RGBA16,
			RGB10_A2,
			RGBA16F,
			DEPTH24,
			DEPTH24_STENCIL8,
			UNKOWN_TEXTURE_FORMAT,
		};
	};

	//
	// Base class for texture objects.
	class Texture : public ResourceObject
	{
	protected:
		//
		// Constructor
		Texture();

		//
		// Constructor
		Texture(TextureFormat::Enum format);
		
		//
		// Binds this texture to the active texture area
		// @param type The type of texture this is, for example: GL_TEXTURE_2D
		void BindToActiveTexture(GLenum type);

	public:
		virtual ~Texture();
		
		//
		// @return This textures bit format.
		TextureFormat::Enum GetFormat() const;

		//
		// @return
		uint32 GetUUID() const;

	protected:
		GLuint mTextureId;
		uint32 mUUID;
		TextureFormat::Enum mFormat;
	};

}
