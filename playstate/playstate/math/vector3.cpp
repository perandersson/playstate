#include "../memory/memory.h"
#include "vector3.h"

#include <cmath>

using namespace playstate;

const Vector3 Vector3::UNIT_X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UNIT_Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UNIT_Z = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NEGATIVE_UNIT_X = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Y = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NEGATIVE_UNIT_Z = Vector3(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);

Vector3::Vector3() : X(0.0f), Y(0.0f), Z(0.0f)
{
}

Vector3::Vector3(float32 xValue, float32 yValue, float32 zValue) : X(xValue), Y(yValue), Z(zValue)
{
}

Vector3::Vector3(const Vector3& vector3) : X(vector3.X), Y(vector3.Y), Z(vector3.Z)
{
}

Vector3::Vector3(const float32* vector3) : X(vector3[0]), Y(vector3[1]), Z(vector3[2])
{
}

Vector3 Vector3::GetNormalized() const
{
	float32 len = Length();
	len = (len != 0 ? len : 1.0f);
	float32 lengthMul = 1.0f / len;

	return Vector3(X * lengthMul, Y * lengthMul, Z * lengthMul);
}

Vector3 Vector3::CrossProduct(const Vector3& vector3) const
{
	return Vector3(	(Y * vector3.Z - Z * vector3.Y),
					(Z * vector3.X - X * vector3.Z),
					(X * vector3.Y - Y * vector3.X));
}

float32 Vector3::Length() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

float32 Vector3::SquaredLength() const
{
	return (X * X + Y * Y + Z * Z);
}

float32 Vector3::DotProduct(const Vector3& vector3) const
{
	return ((vector3.X * X) + (vector3.Y * Y) + (vector3.Z * Z));
}

float32 Vector3::GetAngleDegree(const Vector3& vector3) const
{
	return acosf(((vector3.X * X) + (vector3.Y * Y) + (vector3.Z * Z)) / (vector3.Length() * Length())) * 57.29577951f;
}

float32 Vector3::GetAngleRadians(const Vector3& vector3) const
{
	return acosf(((vector3.X * X) + (vector3.Y * Y) + (vector3.Z * Z)) / (vector3.Length() * Length()));
}

void Vector3::Normalize()
{
	float32 len = Length();
	len = (len != 0.0f ? len : 1.0f);

	float32 lengthMul = 1.0f / len;
	X *= lengthMul;
	Y *= lengthMul;
	Z *= lengthMul;
}

void Vector3::Normalize(float32 distance)
{
	float32 factor = 1.0f / distance;
	X *= factor;
	Y *= factor;
	Z *= factor;
}

void Vector3::Zero()
{
	X = Y = Z = 0.0f;
}

void Vector3::Negate()
{
	X = -X;
	Y = -Y;
	Z = -Z;
}

void Vector3::Set(float32 xValue, float32 yValue, float32 zValue)
{
	X = xValue;
	Y = yValue;
	Z = zValue;
}

void Vector3::Set(const Vector3& vector3)
{
	X = vector3.X;
	Y = vector3.Y;
	Z = vector3.Z;
}

void Vector3::operator += (const Vector3 &vector3)
{
	X += vector3.X;
	Y += vector3.Y;
	Z += vector3.Z;
}

void Vector3::operator -= (const Vector3 &vector3)
{
	X -= vector3.X;
	Y -= vector3.Y;
	Z -= vector3.Z;
}

void Vector3::operator *= (const float32 scalar)
{
	X *= scalar;
	Y *= scalar;
	Z *= scalar;
}

void Vector3::operator *= (const Vector3& scalar)
{
	X *= scalar.X;
	Y *= scalar.Y;
	Z *= scalar.Z;
}

void Vector3::operator /= (const float32 scalar)
{
	if (scalar != 0)
	{
		X /= scalar;
		Y /= scalar;
		Z /= scalar;
	}
}

Vector3 Vector3::operator + (const Vector3& vector3) const
{
	Vector3 vector3Temp;
	vector3Temp.X = X + vector3.X;
	vector3Temp.Y = Y + vector3.Y;
	vector3Temp.Z = Z + vector3.Z;

	return vector3Temp;
}

Vector3 Vector3::operator - (const Vector3& vector3) const
{
	return Vector3(X - vector3.X, Y - vector3.Y, Z - vector3.Z);
}

Vector3 Vector3::operator * (const float32 scalar) const
{
	Vector3 vector3Temp;
	vector3Temp.X = X * scalar;
	vector3Temp.Y = Y * scalar;
	vector3Temp.Z = Z * scalar;

	return vector3Temp;
}

Vector3	Vector3::operator * (const Vector3& scalar) const
{
	Vector3 tmp;
	tmp.X = X * scalar.X;
	tmp.Y = Y * scalar.Y;
	tmp.Z = Z * scalar.Z;
	return tmp;
}

Vector3 Vector3::operator / (const float32 scalar) const
{
	Vector3 vector3Temp;
	if (scalar != 0)
	{
		vector3Temp.X = X / scalar;
		vector3Temp.Y = Y / scalar;
		vector3Temp.Z = Z / scalar;
	}

	return vector3Temp;
}

void Vector3::operator = (const Vector3 &vector3)
{
	X = vector3.X;
	Y = vector3.Y;
	Z = vector3.Z;
}

bool Vector3::operator == (const Vector3 &vector3) const
{
	return abs(X - vector3.X) <= FLT_EPSILON &&
		abs(Y - vector3.Y) <= FLT_EPSILON &&
		abs(Z - vector3.Z) <= FLT_EPSILON;
}

bool Vector3::operator != (const Vector3 &vector3) const
{
	return abs(X - vector3.X) > FLT_EPSILON ||
		abs(Y - vector3.Y) > FLT_EPSILON ||
		abs(Z - vector3.Z) > FLT_EPSILON;
}

Vector3 Vector3::operator - ()
{
	return Vector3(-X, -Y, -Z);
}

bool Vector3::IsZero() const
{
	return abs(X) <= FLT_EPSILON &&
		abs(Y) <= FLT_EPSILON;
}

namespace playstate
{
	int Vector3_Factory(lua_State* L)
	{
		float32 z = lua_tointeger(L, -1); lua_pop(L, 1);
		float32 y = lua_tointeger(L, -1); lua_pop(L, 1);
		float32 x = lua_tointeger(L, -1); lua_pop(L, 1);
		
		float* arr = (float*)lua_newuserdata(L, sizeof(float[3]));
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		return 1;
	}
}
