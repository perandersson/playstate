#include "../memory/memory.h"
#include "point.h"
using namespace playstate;

Point Point::Zero;

Point::Point() : X(0), Y(0)
{
}

Point::Point(const Point& point) : X(point.X), Y(point.Y)
{
}

Point::Point(int x, int y) : X(x), Y(y)
{
}

void Point::operator = (const Point& pt)
{
	X = pt.X;
	Y = pt.Y;
}
