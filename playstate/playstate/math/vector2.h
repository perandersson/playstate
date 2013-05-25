#pragma once

#include "../types.h"

namespace playstate
{
	struct Vector2
	{
		union
		{
			struct
			{
				float32 X;
				float32 Y;
			};

			float32 Points[2];
		};

		Vector2();
		Vector2(float32 x, float32 y);
		Vector2(const Vector2 &vector2);
		Vector2 GetNormalized() const;
		float32 Length() const;
		float32 SquaredLength() const;
		float32 DotProduct(const Vector2& vector2) const;
		float32 GetAngleDegree(const Vector2& vector2) const;
		float32 GetAngleRadians(const Vector2& vector2) const;
		void Normalize();
		void Negate();
		void Set(float32 xValue, float32 yValue);
		void Set(const Vector2& vector2);

		Vector2 operator + (const Vector2 &vector2) const;
		Vector2 operator - (const Vector2 &vector2) const;
		Vector2 operator * (const float32 scalar) const;
		Vector2 operator * (const Vector2 &vector2) const;
		Vector2 operator / (const float32 scalar) const;

		void operator += (const Vector2 &vector2);
		void operator -= (const Vector2 &vector2);
		void operator *= (const float32 scalar);
		void operator /= (const float32 scalar);

		void operator = (const Vector2 &vector2);

		bool operator == (const Vector2 &vector2) const;
		bool operator != (const Vector2 &vector2) const;

		Vector2 operator-();

		static const Vector2 Zero;
		static const Vector2 UnitX; 
		static const Vector2 UnitY;
		static const Vector2 NegativeUnitX;
		static const Vector2 NegativeUnitY;
	};
}
