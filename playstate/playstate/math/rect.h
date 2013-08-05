#pragma once
#include "../types.h"

namespace playstate
{
	struct Rect
	{
		uint32 X;
		uint32 Y;
		uint32 Width;
		uint32 Height;

		//
		// 
		Rect();
		Rect(const Rect& r);
		Rect(uint32 x, uint32 y, uint32 width, uint32 height);
	};
}
