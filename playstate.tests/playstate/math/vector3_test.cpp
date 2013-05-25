#include <playstate/math/vector3.h>
#include "../test.h"
using namespace playstate;

TestCase(Vector3_DefaultConstructor, "Vector3() == Vector3::Zero")
{
	const Vector3 val;

	AssertEquals(val, Vector3::Zero);
}

TestCase(Vector3_ConstructorElements, "Vector3(x, y, z)")
{
	const float32 x = 3.0f;
	const float32 y = 5.0f;
	const float32 z = -201.043f;
	const Vector3 val(x, y, z);

	AssertEquals(val.X, x);
	AssertEquals(val.Y, y);
	AssertEquals(val.Z, z);
}

TestCase(Vector3_Add, "val1 + val2 = expected")
{
	const Vector3 val1(1.0f, 2.0f, 3.0f);
	const Vector3 val2(3.0f, 4.0f, 5.0f);
	const Vector3 expected(4.0f, 6.0f, 8.0f);

	AssertTrue(val1 + val2 == expected);
}

TestCase(Vector3_Sub, "val1 - val2 = expected")
{
	const Vector3 val1(2.0f, 4.0f, 6.0f);
	const Vector3 val2(3.0f, 1.0f, 2.0f);
	const Vector3 expected(-1.0f, 3.0f, 4.0f);

	AssertTrue(val1 - val2 == expected);
}

TestCase(Vector3_Div, "val1 / val2 = expected")
{
	const Vector3 val1(2.0f, 12.0f, 1.0f);
	const float32 val2 = 4.0f;
	const Vector3 expected(0.5f, 3.0f, 0.25f);

	AssertTrue(val1 / val2 == expected);
}

TestCase(Vector3_Mul, "val1 * val2 = expected")
{
	const Vector3 val1(2.0f, 12.0f, 1.0f);
	const Vector3 val2(2.0f, 2.0f, 2.0f);
	const Vector3 expected(4.0f, 24.0f, 2.0f);

	AssertTrue(val1 * val2 == expected);
}

TestCase(Vector3_Length, "|val1| == expected")
{
	const Vector3 val1(3.0f, 4.0f, 5.0f);
	const float32 expected = sqrtf(3*3 + 4*4 + 5*5);

	AssertEquals(val1.Length(), expected);
}

TestCase(Vector3_SquaredLength, "|val1|^2 == expected")
{
	const Vector3 val1(3.0f, 4.0f, 5.0f);
	const float32 expected = 3*3 + 4*4 + 5*5;

	AssertEquals(val1.SquaredLength(), expected);
}

TestCase(Vector3_Normalize, "û == expected")
{
	Vector3 û(3.0f, 4.0f, 5.0f);
	û.Normalize();
	const Vector3 expected = û / û.Length();

	AssertEquals(û, expected);
}

TestCase(Vector3_ZeroAssignment, "val1 == Vector3::Zero")
{
	Vector3 val1(3.0f, 4.0f, 5.0f);
	val1 = Vector3::Zero;

	AssertEquals(val1, Vector3::Zero);
}

TestCase(Vector3_Negate, "val1.Negate() == expected")
{
	Vector3 val1(3.0f, 4.0f, 5.0f);
	val1.Negate();
	const Vector3 expected(-3.0f, -4.0f, -5.0f);

	AssertEquals(val1, expected);
}

TestCase(Vector3_Equals, "(val1 == val2) == true")
{
	const Vector3 val1(3.542f, 12.219f, -32.012f);
	const Vector3 val2(3.542f, 12.219f, -32.012f);
	
	AssertTrue(val1 == val2);
}

TestCase(Vector3_NotEquals, "(val1 == val2) == true")
{
	const Vector3 val1(3.5f, 12.21f, -32.012f);
	const Vector3 val2(3.51f, 12.21f, -32.01f);
	
	AssertTrue(val1 != val2);
}
