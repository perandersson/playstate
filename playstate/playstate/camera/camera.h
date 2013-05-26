#pragma once

#include "frustum.h"
#include "../math/matrix4x4.h"
#include "../math/vector3.h"

namespace playstate
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);
		void SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

		void Move(const Vector3& direction);
		void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

	private:
		void CalculateLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
		
	public:
		// Readonly property for the view matrix
		const Matrix4x4& ViewMatrix;

		// Readonly property for the projection matrix
		const Matrix4x4& ProjectionMatrix;

		// Readonly property for the cameras position vector
		const Vector3& Position;

		// Readonly property for the cameras center vector
		const Vector3& Center;

		// Readonly property for the cameras up vector
		const Vector3& Up;

		// Read-only property for the view frustum of this camera.
		const Frustum& ViewFrustum;

	private:
		Frustum mViewFrustum;
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjection;

		Vector3 mPosition;
		Vector3 mCenter;
		Vector3 mUp;
	};

	//
	// Script integration
	//

	extern int ActiveCamera_SetPerspective(lua_State* L);
	extern int ActiveCamera_LookAt(lua_State* L);
	extern int ActiveCamera_Move(lua_State* L);
	static luaL_Reg ActiveCamera_Methods[] = {
		{ "SetPerspective", ActiveCamera_SetPerspective },
		{ "LookAt", ActiveCamera_LookAt },
		{ "Move", ActiveCamera_Move },
		{ NULL, NULL }
	};
}
