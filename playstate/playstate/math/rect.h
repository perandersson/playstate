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

		inline bool operator == (const Rect& rhs) const {
			return X == rhs.X && Y == rhs.Y && Width == rhs.Width && Height == rhs.Height;
		}

		inline bool operator != (const Rect& rhs) const {
			return X != rhs.X || Y != rhs.Y || Width != rhs.Width || Height != rhs.Height;
		}

		inline void operator = (const Rect& r) {
			X = r.X;
			Y = r.Y;
			Width = r.Width;
			Height = r.Height;
		}
	};
}
