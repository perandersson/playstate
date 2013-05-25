#include <playstate/math/vector2.h>
#include "../test.h"
using namespace playstate;

TestCase(Vector2_DefaultConstructor, "Vector2() == Vector2::ZERO")
{
	const Vector2 val;

	AssertEquals(val, Vector2::ZERO);
}

TestCase(Vector2_Add, "val1 + val2 = expected")
{
	const Vector2 val1(1.0f, 2.0f);
	const Vector2 val2(3.0f, 4.0f);
	const Vector2 expected(4.0f, 6.0f);

	AssertTrue(val1 + val2 == expected);
}

TestCase(Vector2_Sub, "val1 - val2 = expected")
{
	const Vector2 val1(2.0f, 4.0f);
	const Vector2 val2(3.0f, 1.0f);
	const Vector2 expected(-1.0f, 3.0f);

	AssertTrue(val1 - val2 == expected);
}

TestCase(Vector2_Length, "|val1| == expected")
{
	const Vector2 val1(3.0f, 4.0f);
	const float32 expected = sqrtf(3*3 + 4*4);

	AssertEquals(val1.CalcLength(), expected);
}
