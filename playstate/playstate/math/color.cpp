#include "../memory/memory.h"
#include "color.h"

using namespace playstate;

const Color Color::White(1.0f, 1.0f, 1.0f, 1.0f);
const Color Color::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Color Color::Nothing(0.0f, 0.0f, 0.0f, 0.0f);

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

Color Color::HexToRGB(const char* hex)
{
	assert_not_null(hex);
	std::string value;
	if(hex[0] == '#')
		value += &hex[1];
	else
		value += hex;

	if(value.length() == 2) {
		std::string replicateValue = value;
		value += replicateValue;
		value += replicateValue;
		value += replicateValue;
	}

	value = std::string("0x") + value;
	int32 hexbits = 0;
	sscanf(value.c_str(), "%x", &hexbits);
	return Color(
		((hexbits >> 16) & 0xFF) / 255.f,
		((hexbits >> 8) & 0xFF) / 255.f,
		((hexbits >> 0) & 0xFF) / 255.f
	);
}
