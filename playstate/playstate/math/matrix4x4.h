#pragma once

#include "vector3.h"

namespace playstate
{
	struct Quaternion;
	struct ALIGN_DATA(16) Matrix4x4
	{
		union
		{
			struct {
				float32 _11, _12, _13, _14;
				float32 _21, _22, _23, _24;
				float32 _31, _32, _33, _34;
				float32 _41, _42, _43, _44;
			};
			struct {
				float32 Row1[4];
				float32 Row2[4];
				float32 Row3[4];
				float32 Row4[4];
			};
			float32 Values[16];
		};

		Matrix4x4();
		Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
				  float32 m21, float32 m22, float32 m23, float32 m24,
				  float32 m31, float32 m32, float32 m33, float32 m34,
				  float32 m41, float32 m42, float32 m43, float32 m44);
		Matrix4x4(const Matrix4x4 &matrix4);

		void Zero();
		void SetIdentity();
		void Translate(const Vector3& vec);
		float32 Determinant();
		void Transpose();
		void Invert();
		void AxisAndAngleToMatrix(Vector3& vector, float32 angleRadians);
		void QuaternionToMatrix(Quaternion& quaternion);
		void Set(float32 m11, float32 m12, float32 m13, float32 m14,
				 float32 m21, float32 m22, float32 m23, float32 m24,
				 float32 m31, float32 m32, float32 m33, float32 m34,
				 float32 m41, float32 m42, float32 m43, float32 m44);

		//void Set(float *matrix);

		Matrix4x4 operator + (const Matrix4x4& matrix) const;
		Matrix4x4 operator - (const Matrix4x4& matrix) const;
		Matrix4x4 operator * (const Matrix4x4& matrix) const;
		Vector3 operator * (const Vector3& vec3) const;

		const void operator += (const Matrix4x4& matrix);
		const void operator -= (const Matrix4x4& matrix);
		const void operator *= (const Matrix4x4& matrix);

		Matrix4x4 operator * (const float32 scalar) const;
		Matrix4x4 operator / (const float32 scalar) const;
		const void operator *= (const float32 scalar);
		const void operator /= (const float32 scalar);

		void operator = (const Matrix4x4& matrix);
		bool operator == (const Matrix4x4& matrix);
		bool operator != (const Matrix4x4& matrix);

		static const Matrix4x4 IDENTITY;

		static Matrix4x4 Translation(const Vector3& vec);
		static Matrix4x4 Rotation(const Vector3& rot);
		static Matrix4x4 RotationX(float32 angleX);
		static Matrix4x4 RotationY(float32 angleY);
		static Matrix4x4 RotationZ(float32 angleZ);
		static Matrix4x4 Scale(float32 x, float32 y, float32 z);
	};
}
