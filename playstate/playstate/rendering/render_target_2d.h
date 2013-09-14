#pragma once

#include "../types.h"
#include "texture2d.h"

namespace playstate
{

	//
	// Interested into drawing into a texture/bitmap instead of onto the screen? Then this class is for you
	class IRenderTarget2D : public ITexture2D
	{
	public:
		virtual ~IRenderTarget2D() {}
	};
}
