#include "../memory/memory.h"
#include "rect.h"
using namespace playstate;

Rect::Rect()
	: X(0), Y(0), Width(0), Height(0)
{
}

Rect::Rect(const Rect& r)
	: Position(r.Position), Size(r.Size)
{
}

Rect::Rect(const Point& position, const _Size& size)
	: Position(position), Size(size)
{
}

Rect::Rect(int32 x, int32 y, int32 width, int32 height)
	: Position(x, y), Size(width, height)
{
}
