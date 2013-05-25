#include <playstate/math/vector3.h>
#include "../test.h"
using namespace playstate;

TestCase(Vector3_DefaultConstructor, "Vector3() == Vector3::ZERO")
{
	const Vector3 val;

	AssertEquals(val, Vector3::ZERO);
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

TestCase(Vector3_Zero, "val1.Zero() == Vector3::ZERO")
{
	Vector3 val1(3.0f, 4.0f, 5.0f);
	val1.Zero();

	AssertEquals(val1, Vector3::ZERO);
}
