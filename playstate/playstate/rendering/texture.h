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
