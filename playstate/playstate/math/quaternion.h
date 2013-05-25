#pragma once

#include "vector3.h"

namespace playstate
{
	struct Matrix4x4;
	struct Quaternion
	{
		union
		{
			struct
			{
				float32 X;
				float32 Y;
				float32 Z;
				float32 W;
			};

			float32 Values[4];
		};

		Quaternion();
		Quaternion(float32 xValue, float32 yValue, float32 zValue, float32 wValue);
		Quaternion(const Quaternion &quaternion);
		Quaternion(const float32* quaternion);
	
		Vector3	GetVector();
		float32 GetScalar();
		float32 Magnitude();
		void Normalize();

		void Rotate(const Quaternion& quaternion);
		void Roll(float32 angle);
		void Pitch(float32 angle);
		void Yaw(float32 angle);

		float32 GetAngle();

		Vector3	GetAxis() const;

		void QuaternionFromEuler(Vector3& vector3);
		Vector3 GetEuler();
		//Matrix3 Quaternion::QuaternionToMatrix3();
		Matrix4x4 QuaternionToMatrix4();
		//void Quaternion::QuaternionFromMatrix3(Matrix3& matrix3);
		void QuaternionFromMatrix4(Matrix4x4& matrix4);
		void QuaternionFromAxisAngle(Vector3& vector3, float32 angle);

		Quaternion SLERP(Quaternion &quaternion, float32 interpolation);
		Quaternion LERP(Quaternion &quaternion, float32 interpolation);

		void Invert();
		void RotateVector(Vector3& v) const;

		const Quaternion operator + (const Quaternion& quaternion) const;
		const Quaternion operator - (const Quaternion& quaternion) const;
		const Quaternion operator * (const Quaternion& quaternion) const;

		const void operator += (const Quaternion& quaternion);
		const void operator -= (const Quaternion& quaternion);
		const void operator *= (const Quaternion& quaternion);

		const Quaternion operator * (float32 scalar) const;
		const Quaternion operator / (float32 scalar) const;
		const void operator *= (float32 scalar);
		const void operator /= (float32 scalar);

		const Quaternion operator * (const Vector3& vector3) const;
		const void operator *= (const Vector3& vector3);

		const Quaternion operator - () const;
		const Quaternion operator ~ () const;

		const void operator = (const Quaternion& quaternion);
		bool operator == (const Quaternion& quaternion);
		bool operator != (const Quaternion& quaternion);

	private:
		void CalculateW();
	};
}
