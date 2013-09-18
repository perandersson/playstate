#pragma once
#include "../types.h"
#include "point.h"

namespace playstate
{
	
	struct Rect
	{
		typedef Size _Size;

		union 
		{
			struct 
			{
				int32 X;
				int32 Y;
				int32 Width;
				int32 Height;
			};
			struct
			{
				Point Position;
				_Size Size;
			};
			int32 Elements[4];
		};

		//
		// 
		Rect();
		Rect(const Rect& r);
		Rect(const Point& position, const _Size& size);
		Rect(int32 x, int32 y, int32 width, int32 height);

		inline bool operator == (const Rect& rhs) const {
			return X == rhs.X && Y == rhs.Y && Width == rhs.Width && Height == rhs.Height;
		}

		inline bool operator != (const Rect& rhs) const {
			return X != rhs.X || Y != rhs.Y || Width != rhs.Width || Height != rhs.Height;
		}

		inline Rect& operator = (const Rect& r) {
			X = r.X;
			Y = r.Y;
			Width = r.Width;
			Height = r.Height;
			return *this;
		}

		//
		// Return a translated version of this rectangle instance (i.e. moved)
		//
		// @param dir The direction we want to move the new rectangle towards.
		// @return The new rectangle instance
		inline Rect GetTranslated(const Point& dir) const {
			return Rect(X + dir.X, Y + dir.Y, Width, Height);
		}

		//
		// Checks if the supplied point is inside this rectangle instance
		// 
		// @param pt The point we want to check
		// @return TRUE if the supplied point is inside this rect (i.e. NOT intersecting on the border but is INSIDE).
		inline bool IsPointInside(const Point& pt) const {
			if(pt.X > (X) && pt.X < (X + Width)) {
				if(pt.Y > (Y) && pt.Y < (Y + Height)) {
					return true;
				}
			}
			return false;
		}
	};
}
