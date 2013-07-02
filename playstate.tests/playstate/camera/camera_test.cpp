#include <playstate/camera/camera.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(Camera)
{
	UNIT_TEST(Camera_Constructor)
	{
		const Camera camera;

		ASSERT_EQUALS(camera.ViewMatrix, Matrix4x4::Identity);
		ASSERT_EQUALS(camera.ProjectionMatrix, Matrix4x4::Identity);
	}

	UNIT_TEST(Camera_MoveCamera)
	{
		const Vector3 direction(10.0f, 5.0f, 2.0f);
		Camera camera;
		camera.Move(direction);

		ASSERT_EQUALS(camera.Position, direction);
	
		camera.Move(direction);

		ASSERT_EQUALS(camera.Position, direction * 2);
	}
}