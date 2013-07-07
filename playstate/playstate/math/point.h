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
			int32 Points[2];
		};

		Point();
		Point(const Point& point);
		Point(int32 x, int32 y);

		void operator = (const Point& pt);

		static Point Zero;
	};

}

