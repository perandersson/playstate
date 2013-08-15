#include <playstate/camera/camera.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(Camera)
{
	UNIT_TEST(Camera_Constructor)
	{
		const Camera camera;

		ASSERT_EQUALS(camera.GetViewMatrix(), Matrix4x4::Identity);
		ASSERT_EQUALS(camera.GetProjectionMatrix(), Matrix4x4::Identity);
	}

	UNIT_TEST(Camera_MoveCamera)
	{
		const Vector3 direction(10.0f, 5.0f, 2.0f);
		Camera camera;
		camera.Move(direction);

		ASSERT_EQUALS(camera.GetPosition(), direction);
	
		camera.Move(direction);

		ASSERT_EQUALS(camera.GetPosition(), direction * 2);
	}

	UNIT_TEST(OrthographicView)
	{
		const Matrix4x4 mat = Camera::GetOrtho2D(0.0f, 1024.0f, 768.0f, 0.0f);
		const Vector3 pos = Vector3(300.0f, 300.0f, 0.0f);

		Vector3 result = mat * pos;
		ASSERT_TRUE(result.Z == 0.0f);
	}
}