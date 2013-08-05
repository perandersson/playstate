#include "../memory/memory.h"
#include "rect.h"
using namespace playstate;

Rect::Rect()
	: X(0), Y(0), Width(0), Height(0)
{
}

Rect::Rect(const Rect& r)
	: X(r.X), Y(r.Y), Width(r.Width), Height(r.Height)
{
}

Rect::Rect(uint32 x, uint32 y, uint32 width, uint32 height)
	: X(x), Y(y), Width(width), Height(height)
{
}
