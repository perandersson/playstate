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
		float32 CalcLength() const;
		float32 CalcSquaredLength() const;
		float32 DotProduct(const Vector2& vector2) const;
		float32 GetAngleDegree(const Vector2& vector2) const;
		float32 GetAngleRadians(const Vector2& vector2) const;
		void Normalize();
		void Zero();
		void Negate();
		void Set(float32 xValue, float32 yValue);
		void Set(const Vector2& vector2);

		Vector2 operator + (const Vector2 &vector2) const;
		Vector2 operator - (const Vector2 &vector2) const;
		Vector2 operator * (const float32 scalar) const;
		Vector2 operator / (const float32 scalar) const;

		void operator += (const Vector2 &vector2);
		void operator -= (const Vector2 &vector2);
		void operator *= (const float32 scalar);
		void operator /= (const float32 scalar);

		void operator = (const Vector2 &vector2);

		bool operator == (const Vector2 &vector2) const;
		bool operator != (const Vector2 &vector2) const;

		Vector2 operator-();

		static const Vector2 ZERO;
		static const Vector2 UNIT_X; 
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
	};
}
