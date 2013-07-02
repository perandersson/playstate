#pragma once
#include <gl/glew.h>
#include "../types.h"
#include "../resources/resource.h"

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

	public:
		// Read-only property for the format of this texture.
		const TextureFormat::Enum& Format;

		// Read-only property for a unique id for this object.
		const uint32& UniqueId;
		
	protected:
		GLuint mTextureId;
		uint32 mUniqueId;
		TextureFormat::Enum mTextureFormat;
	};

}
