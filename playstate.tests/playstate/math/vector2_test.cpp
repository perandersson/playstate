#include <playstate/math/vector2.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(Vector2)
{
	UNIT_TEST(Vector2_DefaultConstructor)
	{
		const Vector2 val;

		ASSERT_EQUALS(val, Vector2::Zero);
	}

	UNIT_TEST(Vector2_ConstructorElements)
	{
		const float32 x = 3.0f;
		const float32 y = 5.0f;
		const Vector2 val(x, y);
	
		ASSERT_EQUALS(val.X, x);
		ASSERT_EQUALS(val.Y, y);
	}

	UNIT_TEST(Vector2_CopyConstructor)
	{
		const Vector2 expected(3.0f, 5.0f);
		const Vector2 val = expected;
	
		ASSERT_EQUALS(val.X, expected.X);
		ASSERT_EQUALS(val.Y, expected.Y);
	}

	UNIT_TEST(Vector2_Add)
	{
		const Vector2 val1(1.0f, 2.0f);
		const Vector2 val2(3.0f, 4.0f);
		const Vector2 expected(4.0f, 6.0f);

		ASSERT_TRUE(val1 + val2 == expected);
	}

	UNIT_TEST(Vector2_Sub)
	{
		const Vector2 val1(2.0f, 4.0f);
		const Vector2 val2(3.0f, 1.0f);
		const Vector2 expected(-1.0f, 3.0f);

		ASSERT_TRUE(val1 - val2 == expected);
	}

	UNIT_TEST(Vector2_Mul)
	{
		const Vector2 val1(2.0f, 12.0f);
		const Vector2 val2(2.0f, 2.0f);
		const Vector2 expected(4.0f, 24.0f);

		ASSERT_TRUE(val1 * val2 == expected);
	}

	UNIT_TEST(Vector2_Div)
	{
		const Vector2 val1(2.0f, 12.0f);
		const float32 val2 = 4.0f;
		const Vector2 expected(0.5f, 3.0f);

		ASSERT_TRUE(val1 / val2 == expected);
	}

	UNIT_TEST(Vector2_Length)
	{
		const Vector2 val1(3.0f, 4.0f);
		const float32 expected = sqrtf(3*3 + 4*4);

		ASSERT_EQUALS(val1.Length(), expected);
	}

	UNIT_TEST(Vector2_SquaredLength)
	{
		const Vector2 val1(3.0f, 4.0f);
		const float32 expected = 3*3 + 4*4;

		ASSERT_EQUALS(val1.SquaredLength(), expected);
	}

	UNIT_TEST(Vector2_Normalize)
	{
		Vector2 û(3.0f, 4.0f);
		û.Normalize();
		const Vector2 expected = û / û.Length();

		ASSERT_EQUALS(û, expected);
	}

	UNIT_TEST(Vector2_ZeroAssignment)
	{
		Vector2 val1(3.0f, 4.0f);
		val1 = Vector2::Zero;

		ASSERT_EQUALS(val1, Vector2::Zero);
	}

	UNIT_TEST(Vector2_Negate)
	{
		Vector2 val1(3.0f, 4.0f);
		val1.Negate();
		const Vector2 expected(-3.0f, -4.0f);

		ASSERT_EQUALS(val1, expected);
	}

	UNIT_TEST(Vector2_Equals)
	{
		const Vector2 val1(3.542f, 12.219f);
		const Vector2 val2(3.542f, 12.219f);
	
		ASSERT_TRUE(val1 == val2);
	}

	UNIT_TEST(Vector2_NotEquals)
	{
		const Vector2 val1(3.5f, 12.21f);
		const Vector2 val2(3.51f, 12.21f);
	
		ASSERT_TRUE(val1 != val2);
	}
}
