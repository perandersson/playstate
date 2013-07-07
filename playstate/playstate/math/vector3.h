#pragma once

#include "../types.h"

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
		float32 Length() const;
		float32 SquaredLength() const;
		bool IsZero() const;
		float32 DotProduct(const Vector3& vector3) const;
		float32 GetAngleDegree(const Vector3& vector3) const;
		float32 GetAngleRadians(const Vector3& vector3) const;
		void Normalize();
		void Normalize(float32 distance);
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

		static const Vector3 Zero; 
		static const Vector3 UnitX; 
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 NegativeUnitX;
		static const Vector3 NegativeUnitY;
		static const Vector3 NegativeUnitZ;
	};
}
