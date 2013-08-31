#pragma once

#include "frustum.h"
#include "../math/matrix4x4.h"
#include "../script/luam.h"

namespace playstate
{
	//
	//
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);
		void SetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

		void Move(const Vector3& direction);
		void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		//
		// @return This camera's view frustum.
		// @see http://en.wikipedia.org/wiki/Viewing_frustum
		const Frustum& GetViewFrustum() const;

		//
		// @return This camera's view matrix. 
		const Matrix4x4& GetViewMatrix() const;
		const Matrix4x4& GetProjectionMatrix() const;
		const Vector3& GetPosition() const;
		const Vector3& GetUp() const;
		const Vector3& GetCenter() const;

	public:
		//
		// Generate a orthographic projection matrix and returns the result
		//
		// @return
		static Matrix4x4 GetOrtho2D(float32 left, float32 right, float32 bottom, float32 top);

	private:
		void CalculateLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
		
	private:
		Frustum mViewFrustum;
		Matrix4x4 mViewMatrix;
		Matrix4x4 mProjectionMatrix;

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
	extern int ActiveCamera_GetPosition(lua_State* L);
	extern int ActiveCamera_GetUp(lua_State* L);
	static luaL_Reg ActiveCamera_Methods[] = {
		{ "SetPerspective", ActiveCamera_SetPerspective },
		{ "LookAt", ActiveCamera_LookAt },
		{ "Move", ActiveCamera_Move },
		{ "GetPosition", ActiveCamera_GetPosition },
		{ "GetUp", ActiveCamera_GetUp },
		{ NULL, NULL }
	};
}
