#include "../memory/memory.h"
#include "camera.h"
#include "../game/game_runner.h"
#include <string>
#include <cmath>

using namespace playstate;

Camera::Camera()
{
}

Camera::Camera(const Camera& camera)
	: mViewFrustum(camera.mViewFrustum), mViewMatrix(camera.mViewMatrix), mProjectionMatrix(camera.mProjectionMatrix),
	mPosition(camera.mPosition), mCenter(camera.mCenter), mUp(camera.mUp)
{
}

Camera::~Camera()
{
}

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

	mProjectionMatrix = Matrix4x4::Zero;
	mProjectionMatrix._11 = temp / temp2;
	mProjectionMatrix._22 = temp / temp3;
	mProjectionMatrix._31 = (right + left) / temp2;
	mProjectionMatrix._32 = (top + bottom) / temp3;
	mProjectionMatrix._33 = (-farPlane - nearPlane) / temp4;
	mProjectionMatrix._34 = -1.0f;
	mProjectionMatrix._43 = (-temp * farPlane) / temp4;

	mViewFrustum.SetPerspective(nearPlane, farPlane, fov, ratio);
}

void Camera::SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	mProjectionMatrix = GetOrtho2D(left, right, bottom, top);
}

void Camera::Move(const Vector3& direction)
{
	LookAt(mPosition + direction, mCenter + direction, mUp);
}

Matrix4x4 Camera::GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top)
{
	float32 near = -1;
	float32 far = 1;
	
	float32 tx = -((right + left) / (right - left));
	float32 ty = -((top + bottom) / (top - bottom));
	float32 tz = -((far + near) / (far - near));

	Matrix4x4 projection;
	projection._11 = 2.0f / (right - left);
	projection._14 = tx;
	projection._22 = 2.0f / (top - bottom);
	projection._24 = ty;
	projection._33 = -2.0f / (far - near);
	projection._34 = tz;
	projection._44 = 1.0f;
	// TODO: Seriously - need to fix so that matrices doesn't need to be transposed!!!
	projection.Transpose();
	return projection;
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

	mViewMatrix = Matrix4x4(side.X, newUp.X, -forward.X, 0.0f,
		side.Y, newUp.Y, -forward.Y, 0.0f,
		side.Z, newUp.Z, -forward.Z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	mViewMatrix.Translate(eye * -1.0f);

	mViewFrustum.LookAt(eye, center, up);
}

const Frustum& Camera::GetViewFrustum() const
{
	return mViewFrustum;
}

const Matrix4x4& Camera::GetViewMatrix() const
{
	return mViewMatrix;
}

const Matrix4x4& Camera::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

const Vector3& Camera::GetPosition() const
{
	return mPosition;
}

const Vector3& Camera::GetUp() const
{
	return mUp;
}

const Vector3& Camera::GetCenter() const
{
	return mCenter;
}

namespace playstate
{
	int ActiveCamera_SetPerspective(lua_State* L)
	{
		float32 ratio = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 fov = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 farPlane = (float32)lua_tonumber(L, -1); lua_pop(L, 1);
		float32 nearPlane = (float32)lua_tonumber(L, -1); lua_pop(L, 1);

		GameRunner::Get().GetScene().GetActiveCamera().SetPerspective(nearPlane, farPlane, fov, ratio);
		return 0;
	}

	int ActiveCamera_LookAt(lua_State* L)
	{
		Vector3 up = luaM_popvector3(L);
		Vector3 center = luaM_popvector3(L);
		Vector3 eye = luaM_popvector3(L);

		GameRunner::Get().GetScene().GetActiveCamera().LookAt(eye, center, up);
		return 0;
	}

	int ActiveCamera_Move(lua_State* L)
	{
		float* direction = (float*)lua_touserdata(L, -1); lua_pop(L, 1);

		GameRunner::Get().GetScene().GetActiveCamera().Move(Vector3(direction));
		return 0;
	}
	
	int ActiveCamera_GetPosition(lua_State* L)
	{
		return luaM_pushvector3(L, GameRunner::Get().GetScene().GetActiveCamera().GetPosition());
	}

	int ActiveCamera_GetUp(lua_State* L)
	{
		return luaM_pushvector3(L, GameRunner::Get().GetScene().GetActiveCamera().GetUp());
	}

}
