#include <playstate/math/vector3.h>
#include "../test.h"
using namespace playstate;

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
