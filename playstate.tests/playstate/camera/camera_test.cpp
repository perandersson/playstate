#include <playstate/camera/camera.h>
#include "../test.h"
using namespace playstate;

TestCase(Camera_Constructor, "Camera()")
{
	const Camera camera;

	AssertEquals(camera.ViewMatrix, Matrix4x4::Identity);
	AssertEquals(camera.ProjectionMatrix, Matrix4x4::Identity);
}

TestCase(Camera_MoveCamera, "Camera.Move")
{
	const Vector3 direction(10.0f, 5.0f, 2.0f);
	Camera camera;
	camera.Move(direction);

	AssertEquals(camera.Position, direction);
	
	camera.Move(direction);

	AssertEquals(camera.Position, direction * 2);
}
