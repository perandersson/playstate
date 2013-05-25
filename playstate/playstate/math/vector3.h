#pragma once

#include "../types.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	struct Vector3
	{
		union
		{
			struct
			{
				float32 X;
				float32 Y;
				float32 Z;
			};

			float32 Points[3];
		};

		Vector3();
		Vector3(float32 xValue, float32 yValue, float32 zValue);
		Vector3(const Vector3 &vector3);
		Vector3(const float32 *vector3);

		Vector3 GetNormalized() const;
		Vector3 CrossProduct(const Vector3& vector3) const;
		float32 CalcLength() const;
		float32 CalcSquaredLength() const;
		bool IsZero() const;
		float32 DotProduct(const Vector3& vector3) const;
		float32 GetAngleDegree(const Vector3& vector3) const;
		float32 GetAngleRadians(const Vector3& vector3) const;
		void Normalize();
		void Normalize(float32 distance);
		void Zero();
		void Negate();
		void Set(float32 xValue, float32 yValue, float32 zValue);
		void Set(const Vector3& vector3);

		Vector3 operator + (const Vector3& vector3) const;
		Vector3 operator - (const Vector3& vector3) const;
		Vector3	operator * (const float32 scalar) const;
		Vector3	operator * (const Vector3& scalar) const;
		Vector3 operator / (const float32 scalar) const;

		void operator += (const Vector3& vector3);
		void operator -= (const Vector3& vector3);
		void operator *= (const float32 scalar);
		void operator *= (const Vector3& scalar);
		void operator /= (const float32 scalar);

		void operator = (const Vector3 &vector3);

		bool operator == (const Vector3 &vector3) const;
		bool operator != (const Vector3 &vector3) const;

		Vector3 operator - ();

		static const Vector3 ZERO; 
		static const Vector3 UNIT_X; 
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
	};
	
	//
	// Script integration
	//

	extern int Vector3_Factory(lua_State* L);
	static luaL_Reg Vector3_Methods[] = {
		{ "__call", Vector3_Factory },
		{ NULL, NULL }
	};
}
