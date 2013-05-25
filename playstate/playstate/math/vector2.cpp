#include "../memory/memory.h"
#include "vector2.h"

#include <cmath>

using namespace playstate;

const Vector2 Vector2::ZERO = Vector2(0.0f, 0.0f);
const Vector2 Vector2::UNIT_X = Vector2(1.0f, 0.0f);
const Vector2 Vector2::UNIT_Y = Vector2(0.0f, 1.0f);
const Vector2 Vector2::NEGATIVE_UNIT_X = Vector2(-1.0f, 0.0f);
const Vector2 Vector2::NEGATIVE_UNIT_Y = Vector2(0.0f, -1.0f);

Vector2::Vector2() : X(0.0f), Y(0.0f)
{
}

Vector2::Vector2(float32 x, float32 y) : X(x), Y(y)
{
}

Vector2::Vector2(const Vector2 &vector2) : X(vector2.X), Y(vector2.Y)
{
}

Vector2 Vector2::GetNormalized() const
{
	Vector2 normalizedVector;

	float32 len = Length();
	len = (len != 0 ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	normalizedVector.X = X * lengthMul;
	normalizedVector.Y = Y * lengthMul;

	return normalizedVector;
}

float32 Vector2::Length() const
{
	return sqrt((X*X) + (Y*Y));
}

float32 Vector2::SquaredLength() const
{
	return ((X * X) + (Y * Y));
}

float32 Vector2::DotProduct(const Vector2& vector2) const
{
	return ((vector2.X * X) + (vector2.Y * Y));
}

float32 Vector2::GetAngleDegree(const Vector2& vector2) const
{
	return acosf(((vector2.X * X) + (vector2.Y * Y)) / (vector2.Length() * Length())) * 57.29577951f;
}

float32 Vector2::GetAngleRadians(const Vector2& vector2) const
{
	return acosf(((vector2.X * X) + (vector2.Y * Y)) / (vector2.Length() * Length()));
}

void Vector2::Normalize()
{
	float32 len = Length();
	len = (len != 0 ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	X *= lengthMul;
	Y *= lengthMul;
}

void Vector2::Zero()
{
	X = 0.0;
	Y = 0.0;
}

void Vector2::Negate()
{
	X = -X;
	Y = -Y;
}

void Vector2::Set(float32 xValue, float32 yValue)
{
	X = xValue;
	Y = yValue;
}

void Vector2::Set(const Vector2& vector2)
{
	X = vector2.X;
	Y = vector2.Y;
}

void Vector2::operator += (const Vector2 &vector2)
{
	X += vector2.X;
	Y += vector2.Y;
}

void Vector2::operator -= (const Vector2 &vector2)
{
	X -= vector2.X;
	Y -= vector2.Y;
}

void Vector2::operator *= (const float32 scalar)
{
	X *= scalar;
	Y *= scalar;
}

void Vector2::operator /= (const float32 scalar)
{
	if(scalar != 0) {
		X /= scalar;
		Y /= scalar;
	}
}

Vector2 Vector2::operator + (const Vector2 &vector2) const
{
	Vector2 vector2Temp;
	vector2Temp.X = X + vector2.X;
	vector2Temp.Y = Y + vector2.Y;

	return vector2Temp;
}

Vector2 Vector2::operator - (const Vector2 &vector2) const
{
	Vector2 vector2Temp;
	vector2Temp.X = X - vector2.X;
	vector2Temp.Y = Y - vector2.Y;

	return vector2Temp;
}

Vector2 Vector2::operator * (const float32 scalar) const
{
	Vector2 vector2Temp;
	vector2Temp.X = X * scalar;
	vector2Temp.Y = Y * scalar;

	return vector2Temp;
}

Vector2	Vector2::operator * (const Vector2& scalar) const
{
	Vector2 tmp;
	tmp.X = X * scalar.X;
	tmp.Y = Y * scalar.Y;
	return tmp;
}

Vector2 Vector2::operator / (const float32 scalar) const
{
	Vector2 vector2Temp;
	if(scalar != 0) {
		vector2Temp.X = X / scalar;
		vector2Temp.Y = Y / scalar;
	}

	return vector2Temp;
}

void Vector2::operator = (const Vector2 &vector2)
{
	X = vector2.X;
	Y = vector2.Y;
}

bool Vector2::operator == (const Vector2 &vector2) const
{
	return abs(X - vector2.X) <= FLT_EPSILON &&
		abs(Y - vector2.Y) <= FLT_EPSILON;
}

bool Vector2::operator != (const Vector2 &vector2) const
{
	return abs(X - vector2.X) > FLT_EPSILON ||
		abs(Y - vector2.Y) > FLT_EPSILON;
}

Vector2 Vector2::operator - ()
{
	return Vector2(-X, -Y);
}
