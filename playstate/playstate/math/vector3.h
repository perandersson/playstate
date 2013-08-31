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

		inline Vector3 operator + (const Vector3& vector3) const {
			return Vector3(X + vector3.X, Y + vector3.Y, Z + vector3.Z);
		}

		inline Vector3 operator - (const Vector3& vector3) const {
			return Vector3(X - vector3.X, Y - vector3.Y, Z - vector3.Z);
		}

		inline Vector3 operator * (const float32 scalar) const {
			return Vector3(X * scalar, Y * scalar, Z * scalar);
		}

		inline Vector3 operator * (const Vector3& scalar) const {
			return Vector3(X * scalar.X, Y * scalar.Y, Z * scalar.Z);
		}

		inline Vector3 operator / (float32 scalar) const {
			assert(scalar != 0.0f && "You are not allowed to divide by 0");
			scalar = 1.0f / scalar;
			return Vector3(X * scalar, Y * scalar, Z * scalar);
		}

		inline Vector3& operator += (const Vector3& vector3) {
			X += vector3.X;
			Y += vector3.Y;
			Z += vector3.Z;
			return *this;
		}

		inline Vector3& operator -= (const Vector3& vector3) {
			X -= vector3.X;
			Y -= vector3.Y;
			Z -= vector3.Z;
			return *this;
		}

		inline Vector3& operator *= (const float32 scalar) {
			X *= scalar;
			Y *= scalar;
			Z *= scalar;
			return *this;
		}

		inline Vector3& operator *= (const Vector3& scalar) {
			X *= scalar.X;
			Y *= scalar.Y;
			Z *= scalar.Z;
			return *this;
		}

		inline Vector3& operator /= (float32 scalar) {
			if(scalar != 0.0f) {
				scalar = 1.0f / scalar;
				X *= scalar;
				Y *= scalar;
				Z *= scalar;
			}
			return *this;
		}

		inline void operator = (const Vector3 &vector3) {
			X = vector3.X;
			Y = vector3.Y;
			Z = vector3.Z;
		}

		inline bool operator == (const Vector3 &vector3) const {
			return abs(X - vector3.X) <= FLT_EPSILON &&
				abs(Y - vector3.Y) <= FLT_EPSILON &&
				abs(Z - vector3.Z) <= FLT_EPSILON;
		}

		inline bool operator != (const Vector3 &vector3) const {
			return abs(X - vector3.X) > FLT_EPSILON ||
				abs(Y - vector3.Y) > FLT_EPSILON ||
				abs(Z - vector3.Z) > FLT_EPSILON;
		}

		inline Vector3 operator - () {
			return Vector3(-X, -Y, -Z);
		}

		static const Vector3 Zero; 
		static const Vector3 UnitX; 
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 NegativeUnitX;
		static const Vector3 NegativeUnitY;
		static const Vector3 NegativeUnitZ;
	};
}
