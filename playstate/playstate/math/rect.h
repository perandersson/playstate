#pragma once
#include "../types.h"

namespace playstate
{
	struct Rect
	{
		float32 X;
		float32 Y;
		float32 Width;
		float32 Height;

		//
		// 
		Rect();
		Rect(const Rect& r);
		Rect(float32 x, float32 y, float32 width, float32 height);

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
