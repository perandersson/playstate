#pragma once

#include "texture.h"
#include "../math/point.h"

namespace playstate
{
	class IFileSystem;
	
	//
	// 
	class ITexture2D : public ITexture
	{
	public:
		virtual ~ITexture2D() {}

		//
		// @return the size of this 2d texture
		virtual const Size& GetSize() const = 0;
	};
}

