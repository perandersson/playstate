#pragma once

namespace playstate
{
	struct Point
	{
		union
		{
			struct
			{
				int X;
				int Y;
			};
			int Points[2];
		};

		Point();
		Point(const Point& point);
		Point(int x, int y);

		void operator = (const Point& pt);

		static Point Zero;
	};

}

