#include "../memory/memory.h"
#include "color.h"

using namespace playstate;

const Color Color::WHITE(1, 1, 1, 1);
const Color Color::BLACK(0, 0, 0, 1);
const Color Color::NOTHING(0, 0, 0, 0);

Color::Color() : Red(0), Green(0), Blue(0), Alpha(0)
{
}

Color::Color(float32 c) : Red(c), Green(c), Blue(c), Alpha(c)
{
}

Color::Color(float32 r, float32 g, float32 b) : Red(r), Green(g), Blue(b), Alpha(1)
{
}

Color::Color(float32 r, float32 g, float32 b, float32 a) : Red(r), Green(g),Blue(b), Alpha(a)
{
}

Color::Color(const Color& c) : Red(c.Red), Green(c.Green), Blue(c.Blue), Alpha(c.Alpha)
{
}

void Color::Set(const Color& c)
{
	Red = c.Red;
	Green = c.Green;
	Blue = c.Blue;
	Alpha = c.Alpha;
}

bool Color::operator == (const Color& c) const
{
	return abs(c.Red - Red) <= FLT_EPSILON &&
		abs(c.Green - Green) <= FLT_EPSILON &&
		abs(c.Blue - Blue) <= FLT_EPSILON &&
		abs(c.Alpha - Alpha) <= FLT_EPSILON;
}

bool Color::operator != (const Color& c) const
{
	return abs(c.Red - Red) > FLT_EPSILON ||
		abs(c.Green - Green) > FLT_EPSILON ||
		abs(c.Blue - Blue) > FLT_EPSILON ||
		abs(c.Alpha - Alpha) > FLT_EPSILON;
}

void Color::operator = (const Color &c)
{
	Red = c.Red;
	Green = c.Green;
	Blue = c.Blue;
	Alpha = c.Alpha;
}
