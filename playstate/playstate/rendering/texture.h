#pragma once

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
			BGR,
			BGRA,
			DEPTH24,
			DEPTH24_STENCIL8,
		};
	};
	
	class MinFilter
	{
	public:
		enum Enum {
			NEAREST = 0,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR,

			NUM_MIN_FILTERS
		};

		static const Enum Default = LINEAR;
	};

	class MagFilter
	{
	public:
		enum Enum {
			NEAREST = 0,
			LINEAR,

			NUM_MAG_FILTERS
		};

		static const Enum Default = LINEAR;
	};

	class TextureWrap
	{
	public:
		enum Enum {
			CLAMP_TO_EDGE = 0,
			CLAMP_TO_BORDER,
			MIRRORED_REPEAT,
			REPEAT,

			NUM_TEXTURE_WRAPS
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
