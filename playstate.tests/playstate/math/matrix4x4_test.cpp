#include <playstate/math/matrix4x4.h>
#include "../test.h"
using namespace playstate;

TestCase(Matrix4x4_DefaultConstructorIsIdentityMatrix, "Matrix4x4() == Matrix4x4::Identity")
{
	const Matrix4x4 value;

	AssertEquals(value, Matrix4x4::Identity);
}
