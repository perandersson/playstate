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
			R,
			RGB, 
			RGBA,
			RGBA8,
			RGBA12,
			RGBA16,
			RGB10_A2,
			RGBA16F,
			DEPTH24,
			DEPTH24_STENCIL8,
		};
	};
	
	class MinFilter
	{
	public:
		enum Enum {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			UNKNOWN = 0
		};

		static const Enum Default = LINEAR;
	};

	class MagFilter
	{
	public:
		enum Enum {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			UNKNOWN = 0
		};

		static const Enum Default = LINEAR;
	};

	class TextureWrap
	{
	public:
		enum Enum {
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT,
			UNKNOWN = 0
		};

		static const Enum Default = REPEAT;
	};

	//
	// 
	class ITexture : public ResourceObject
	{
	public:
		virtual ~ITexture() {}
		
		//
		// @return This textures bit format.
		virtual TextureFormat::Enum GetFormat() const = 0;
	};

}
