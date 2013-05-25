#include <playstate/math/vector2.h>
#include "../test.h"
using namespace playstate;

TestCase(Vector2_DefaultConstructor, "Vector2() == Vector2::Zero")
{
	const Vector2 val;

	AssertEquals(val, Vector2::Zero);
}

TestCase(Vector2_ConstructorElements, "Vector2(x, y)")
{
	const float32 x = 3.0f;
	const float32 y = 5.0f;
	const Vector2 val(x, y);
	
	AssertEquals(val.X, x);
	AssertEquals(val.Y, y);
}

TestCase(Vector2_CopyConstructor, "Vector2(x, y)")
{
	const Vector2 expected(3.0f, 5.0f);
	const Vector2 val = expected;
	
	AssertEquals(val.X, expected.X);
	AssertEquals(val.Y, expected.Y);
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

TestCase(Vector2_Mul, "val1 * val2 = expected")
{
	const Vector2 val1(2.0f, 12.0f);
	const Vector2 val2(2.0f, 2.0f);
	const Vector2 expected(4.0f, 24.0f);

	AssertTrue(val1 * val2 == expected);
}

TestCase(Vector2_Div, "val1 / val2 = expected")
{
	const Vector2 val1(2.0f, 12.0f);
	const float32 val2 = 4.0f;
	const Vector2 expected(0.5f, 3.0f);

	AssertTrue(val1 / val2 == expected);
}

TestCase(Vector2_Length, "|val1| == expected")
{
	const Vector2 val1(3.0f, 4.0f);
	const float32 expected = sqrtf(3*3 + 4*4);

	AssertEquals(val1.Length(), expected);
}

TestCase(Vector2_SquaredLength, "|val1|^2 == expected")
{
	const Vector2 val1(3.0f, 4.0f);
	const float32 expected = 3*3 + 4*4;

	AssertEquals(val1.SquaredLength(), expected);
}

TestCase(Vector2_Normalize, "û == expected")
{
	Vector2 û(3.0f, 4.0f);
	û.Normalize();
	const Vector2 expected = û / û.Length();

	AssertEquals(û, expected);
}

TestCase(Vector2_ZeroAssignment, "val1.Zero() == Vector2::Zero")
{
	Vector2 val1(3.0f, 4.0f);
	val1 = Vector2::Zero;

	AssertEquals(val1, Vector2::Zero);
}

TestCase(Vector2_Negate, "val1.Negate() == expected")
{
	Vector2 val1(3.0f, 4.0f);
	val1.Negate();
	const Vector2 expected(-3.0f, -4.0f);

	AssertEquals(val1, expected);
}

TestCase(Vector2_Equals, "(val1 == val2) == true")
{
	const Vector2 val1(3.542f, 12.219f);
	const Vector2 val2(3.542f, 12.219f);
	
	AssertTrue(val1 == val2);
}

TestCase(Vector2_NotEquals, "(val1 == val2) == true")
{
	const Vector2 val1(3.5f, 12.21f);
	const Vector2 val2(3.51f, 12.21f);
	
	AssertTrue(val1 != val2);
}
