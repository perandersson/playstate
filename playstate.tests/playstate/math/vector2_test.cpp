#include <playstate/math/vector2.h>
#include "../test.h"
using namespace playstate;

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
