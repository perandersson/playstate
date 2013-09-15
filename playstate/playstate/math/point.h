#pragma once
#include "../types.h"

namespace playstate
{
	struct Point
	{
		union
		{
			struct
			{
				int32 X;
				int32 Y;
			};
			struct
			{
				int32 Width;
				int32 Height;
			};
			int32 Points[2];
		};

		Point();
		Point(const Point& point);
		Point(int32 x, int32 y);

		inline Point& operator = (const Point& pt) {
			X = pt.X;
			Y = pt.Y;
			return *this;
		}

		inline bool operator == (const Point& pt) const {
			return X == pt.X && Y == pt.Y;
		}

		inline bool operator != (const Point& pt) const {
			return X != pt.X || Y != pt.Y;
		}

		inline bool operator >= (const Point& pt) const {
			return X >= pt.X && Y >= pt.Y;
		}
		
		inline bool operator <= (const Point& pt) const {
			return X <= pt.X && Y <= pt.Y;
		}

		inline Point operator + (const Point& rhs) const {
			return Point(X + rhs.X, Y + rhs.Y);
		}

		inline Point operator - (const Point& rhs) const {
			return Point(X - rhs.X, Y - rhs.Y);
		}

		static Point Zero;
	};

	typedef Point Size;
}

