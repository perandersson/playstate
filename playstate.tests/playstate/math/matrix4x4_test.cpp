#include <playstate/math/matrix4x4.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(Matrix4x4)
{
	UNIT_TEST(Matrix4x4_DefaultConstructorIsIdentityMatrix)
	{
		const Matrix4x4 value;

		ASSERT_EQUALS(value, Matrix4x4::Identity);
	}
}
