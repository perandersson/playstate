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

Rect::Rect(float32 x, float32 y, float32 width, float32 height)
	: X(x), Y(y), Width(width), Height(height)
{
}
