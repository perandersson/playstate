#include "../memory/memory.h"
#include "camera.h"
#include "../game/game_runner.h"
#include <string>
#include <cmath>

using namespace playstate;

Camera::Camera() : ViewMatrix(mViewMatrix), ProjectionMatrix(mProjection), 
	Position(mPosition), Center(mCenter), Up(mUp), ViewFrustum(mViewFrustum), mInvalidated(false)
{}

Camera::~Camera()
{}

void Camera::SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio)
{
	// http://www.opengl.org/wiki/GluPerspective_code

    float32 ymax = nearPlane * tanf(fov * (float)(M_PI / 360.0));
    float32 xmax = ymax * ratio;

	float32 left = -xmax;
	float32 right = xmax;
	float32 bottom = -ymax;
	float32 top = ymax;

	float32 temp = 2.0f * nearPlane;
	float32 temp2 = right - left;
	float32 temp3 = top - bottom;
	float32 temp4 = farPlane - nearPlane;

	mProjection = Matrix4x4::Zero;
	mProjection._11 = temp / temp2;
	mProjection._22 = temp / temp3;
	mProjection._31 = (right + left) / temp2;
	mProjection._32 = (top + bottom) / temp3;
	mProjection._33 = (-farPlane - nearPlane) / temp4;
	mProjection._34 = -1.0f;
	mProjection._43 = (-temp * farPlane) / temp4;

	mViewFrustum.SetPerspective(nearPlane, farPlane, fov, ratio);
}

void Camera::SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	float32 near = -1;
	float32 far = 1;
	
	float32 tx = -((right + left) / (right - left));
	float32 ty = -((top + bottom) / (top - bottom));
	float32 tz = -((far + near) / (far - near));

	mProjection = Matrix4x4::Zero;

	mProjection._11 = 2.0f / (right - left);
	mProjection._14 = tx;
	mProjection._22 = 2.0f / (top - bottom);
	mProjection._24 = ty;
	mProjection._33 = -2.0f / (far - near);
	mProjection._34 = tz;
	mProjection._44 = 1.0f;
	// TODO: Seriously - need to fix so that matrices doesn't need to be transposed!!!
	mProjection.Transpose();
}

void Camera::Move(const Vector3& direction)
{
	LookAt(mPosition + direction, mCenter + direction, mUp);
}

void Camera::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	mPosition = eye;
	mCenter = center;
	mUp = up;

	// http://www.opengl.org/wiki/GluLookAt_code

	const Vector3 forward = (center - eye).GetNormalized();
	const Vector3 side = (forward.CrossProduct(up)).GetNormalized();
	const Vector3 newUp = side.CrossProduct(forward).GetNormalized();

	mViewMatrix.Set(side.X, newUp.X, -forward.X, 0.0f,
		side.Y, newUp.Y, -forward.Y, 0.0f,
		side.Z, newUp.Z, -forward.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	mViewMatrix.Translate(eye * -1.0f);

	mViewFrustum.LookAt(eye, center, up);
}

namespace playstate
{
	int ActiveCamera_SetPerspective(lua_State* L)
	{
		float32 ratio = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 fov = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 farPlane = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 nearPlane = (float32)lua_tonumber(L, -1); lua_pop(L, 1);

		GameRunner::Get().ActiveScene.ActiveCamera.SetPerspective(nearPlane, farPlane, fov, ratio);
		return 0;
	}

	int ActiveCamera_LookAt(lua_State* L)
	{
		float* up = (float*)lua_touserdata(L, -1); lua_pop(L, 1);
		float* center = (float*)lua_touserdata(L, -1); lua_pop(L, 1);
		float* eye = (float*)lua_touserdata(L, -1); lua_pop(L, 1);

		GameRunner::Get().ActiveScene.ActiveCamera.LookAt(Vector3(eye), Vector3(center), Vector3(up));
		return 0;
	}
}
