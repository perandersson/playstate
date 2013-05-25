#include <playstate/camera/camera.h>
#include "../test.h"
using namespace playstate;

TestCase(Camera_Constructor, "Camera()")
{
	const Camera camera;

	AssertEquals(camera.ViewMatrix, Matrix4x4::Identity);
	AssertEquals(camera.ProjectionMatrix, Matrix4x4::Identity);
}
