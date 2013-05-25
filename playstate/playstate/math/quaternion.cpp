#include "../memory/memory.h"
#include "quaternion.h"
#include "matrix4x4.h"

#include <cmath>

using namespace playstate;

Quaternion::Quaternion()
{
	Values[0] = 0.0;
	Values[1] = 0.0;
	Values[2] = 0.0;
	Values[3] = 1.0;
}

Quaternion::Quaternion(float32 xValue, float32 yValue, float32 zValue, float32 wValue)
{
	Values[0] = xValue;
	Values[1] = yValue;
	Values[2] = zValue;
	Values[3] = wValue;
}

Quaternion::Quaternion(const Quaternion &quaternion)
{
	Values[0] = quaternion.Values[0];
	Values[1] = quaternion.Values[1];
	Values[2] = quaternion.Values[2];
	Values[3] = quaternion.Values[3];
}

Quaternion::Quaternion(const float* quaternion)
{
	Values[0] = quaternion[0];
	Values[1] = quaternion[1];
	Values[2] = quaternion[2];
	Values[3] = quaternion[3];
}

Vector3 Quaternion::GetVector()
{
	return Vector3(Values);
}

float32 Quaternion::GetScalar()
{
	return Values[3];
}

float32 Quaternion::Magnitude()
{
	return sqrt(Values[0] * Values[0] + Values[1] * Values[1] + Values[2] * Values[2] + Values[3] * Values[3]);
	//return sqrt(pow(Values[0], 2) + pow(Values[1], 2) + pow(Values[2], 2) + pow(Values[3], 2));
}

void Quaternion::Normalize()
{
	float32 inverseMagnitude = 1.0f / Magnitude();

	Values[0] *= inverseMagnitude;
	Values[1] *= inverseMagnitude;
	Values[2] *= inverseMagnitude;
	Values[3] *= inverseMagnitude;
}

void Quaternion::Rotate(const Quaternion& quaternion)
{
	Quaternion tempQuaternion(quaternion * (*this) * ~quaternion);
	*this = tempQuaternion;
}

void Quaternion::Roll(float32 angle)
{
	Vector3 vector3 = GetEuler();
	vector3.X += angle;
	QuaternionFromEuler(vector3);
}

void Quaternion::Pitch(float32 angle)
{
	Vector3 vector3 = GetEuler();
	vector3.Y += angle;
	QuaternionFromEuler(vector3);
}

void Quaternion::Yaw(float32 angle)
{
	Vector3 vector3 = GetEuler();
	vector3.Z += angle;
	QuaternionFromEuler(vector3);
}

float32 Quaternion::GetAngle()
{
	return 2.0f*acosf(Values[3]);
}

Vector3 Quaternion::GetAxis() const
{
	Vector3 v(Values[0], Values[1], Values[2]);
	v.Normalize();
	return v;
}
		
void Quaternion::QuaternionFromEuler(Vector3& vector3)
{
	float32 xValue = vector3.X;
	float32 yValue = vector3.Y;
	float32 zValue = vector3.Z;

	float32 cosX = cosf(xValue * 0.5f);
	float32 sinX = sinf(xValue * 0.5f);
	float32 cosY = cosf(yValue * 0.5f);
	float32 sinY = sinf(yValue * 0.5f);
	float32 cosZ = cosf(zValue * 0.5f);
	float32 sinZ = sinf(zValue * 0.5f);

	Values[3] = cosX * cosY * cosZ + sinX * sinY * sinZ;
	Values[0] = sinX * cosY * cosZ - cosX * sinY * sinZ;
	Values[1] = cosX * sinY * cosZ + sinX * cosY * sinZ;
	Values[2] = cosX * cosY * sinZ - sinX * sinY * cosZ;
}

Vector3 Quaternion::GetEuler()
{
	float32 matrix11 = pow(Values[3], 2) + pow(Values[0], 2) - pow(Values[1], 2) - pow(Values[2], 2);
	float32 matrix12 = (2 * Values[0] * Values[1]) + (2 * Values[3] * Values[2]);
	float32 matrix13 = (2 * Values[0] * Values[2]) - (2 * Values[3] * Values[1]);
	float32 matrix23 = (2 * Values[1] * Values[2]) + (2 * Values[3] * Values[0]);
	float32 matrix33 = pow(Values[3], 2) + pow(Values[0], 2) - pow(Values[1], 2) + pow(Values[2], 2);

	return Vector3(atan2f(matrix23, matrix33), asinf(-matrix13), atan2f(matrix12, matrix11));
}

/*Matrix3 Quaternion::QuaternionToMatrix3()
{
	Matrix3 matrix3;
	float qx = Values[0];
	float qy = Values[1];
	float qz = Values[2];
	float qw = Values[3];			
	matrix3.Set((1 - 2 * qy * qy - 2 * qz * qz),
		(2 * qx * qy - 2 * qz * qw),
		(2 * qx * qz + 2 * qy * qw),

		(2 * qx * qy + 2 * qz * qw),
		(1 - 2 * qx * qx - 2 * qz * qz),
		(2 * qy * qz - 2 * qx * qw),

		(2 * qx * qz - 2 * qy * qw),
		(2 * qy * qz + 2 * qx * qw),
		(1 - 2 * qx * qx - 2 * qy * qy));

	return matrix3;
}*/

Matrix4x4 Quaternion::QuaternionToMatrix4()
{
	Matrix4x4 matrix4;

	float32 qx = Values[0];
	float32 qy = Values[1];
	float32 qz = Values[2];
	float32 qw = Values[3];
	matrix4.Set((1 - 2 * qy * qy - 2 * qz * qz),
		(2 * qx * qy - 2 * qz * qw),
		(2 * qx * qz + 2 * qy * qw),
		0,

		(2 * qx * qy + 2 * qz * qw),
		(1 - 2 * qx * qx - 2 * qz * qz),
		(2 * qy * qz - 2 * qx * qw),
		0,

		(2 * qx * qz - 2 * qy * qw),
		(2 * qy * qz + 2 * qx * qw),
		(1 - 2 * qx * qx - 2 * qy * qy),
		0,
				
		0,
		0,
		0,
		1);

	return matrix4;
}
/*
void Quaternion::QuaternionFromMatrix3(Matrix3& matrix3)
{
	const float *matrix = matrix3.Get();
	float traceOfMatrix = matrix[0] + matrix[4] + matrix[8] + 1.0f;

	if (traceOfMatrix > 0.0f)
	{
		float sTrace = 0.5f / sqrt(traceOfMatrix);
		Values[0] = (matrix[7] - matrix[5]) * sTrace;
		Values[1] = (matrix[2] - matrix[6]) * sTrace;
		Values[2] = (matrix[3] - matrix[1]) * sTrace;
		Values[3] = 0.25f / sTrace;
	}
	else if (matrix[0] > matrix[4] && matrix[0] > matrix[8])
	{
		float sTrace = 1.0f / (sqrt(matrix[0] - matrix[4] - matrix[8] + 1.0f) * 2.0f);
		Values[0] = 0.5f * sTrace;
		Values[1] = (matrix[1] - matrix[3]) * sTrace;
		Values[2] = (matrix[2] - matrix[6]) * sTrace;
		Values[3] = (matrix[5] - matrix[7]) * sTrace;
	}
	else if (matrix[0] > matrix[8])
	{
		float sTrace = 1.0f / (sqrt(matrix[4] - matrix[0] - matrix[8] + 1.0f) * 2.0f);
		Values[0] = (matrix[1] - matrix[3]) * sTrace;
		Values[1] = 0.5f * sTrace;
		Values[2] = (matrix[5] - matrix[7]) * sTrace;
		Values[3] = (matrix[2] - matrix[6]) * sTrace;
	}
	else
	{
		float sTrace = 1.0f / (sqrt(matrix[8] - matrix[0] - matrix[4] + 1.0f) * 2.0f);
		Values[0] = (matrix[2] - matrix[6]) * sTrace;
		Values[1] = (matrix[5] - matrix[7]) * sTrace;
		Values[2] = 0.5f * sTrace;
		Values[3] = (matrix[1] - matrix[3]) * sTrace;
	}
}
*/
void Quaternion::QuaternionFromMatrix4(Matrix4x4& matrix4)
{
	const float32 *matrix = matrix4.Values;
	float32 traceOfMatrix = matrix[0] + matrix[5] + matrix[10] + 1.0f;

	if (traceOfMatrix > 0.0f)
	{
		float32 sTrace = 0.5f / sqrt(traceOfMatrix);
		Values[0] = (matrix[9] - matrix[6]) * sTrace;
		Values[1] = (matrix[2] - matrix[8]) * sTrace;
		Values[2] = (matrix[4] - matrix[1]) * sTrace;
		Values[3] = 0.25f / sTrace;
	}
	else if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
	{
		float32 sTrace = 1.0f / (sqrt(matrix[0] - matrix[5] - matrix[10] + 1.0f) * 2.0f);
		Values[0] = 0.5f * sTrace;
		Values[1] = (matrix[1] - matrix[4]) * sTrace;
		Values[2] = (matrix[2] - matrix[8]) * sTrace;
		Values[3] = (matrix[6] - matrix[9]) * sTrace;
	}
	else if (matrix[0] > matrix[10])
	{
		float32 sTrace = 1.0f / (sqrt(matrix[5] - matrix[0] - matrix[10] + 1.0f) * 2.0f);
		Values[0] = (matrix[1] - matrix[7]) * sTrace;
		Values[1] = 0.5f * sTrace;
		Values[2] = (matrix[6] - matrix[9]) * sTrace;
		Values[3] = (matrix[2] - matrix[8]) * sTrace;
	}
	else
	{
		float32 sTrace = 1.0f / (sqrt(matrix[10] - matrix[0] - matrix[5] + 1.0f) * 2.0f);
		Values[0] = (matrix[2] - matrix[8]) * sTrace;
		Values[1] = (matrix[6] - matrix[9]) * sTrace;
		Values[2] = 0.5f * sTrace;
		Values[3] = (matrix[1] - matrix[7]) * sTrace;
	}
}

void Quaternion::QuaternionFromAxisAngle(Vector3& vector3, float32 angle)
{
	vector3.Normalize();

	float32 s = sinf(angle * 0.5f);
	Values[0] = vector3.X * s;
	Values[1] = vector3.Y * s;
	Values[2] = vector3.X * s;
	Values[3] = cosf(angle * 0.5f);
}

Quaternion Quaternion::SLERP(Quaternion &quaternion, float32 interpolation)
{
	const float32* q1 = Values;
	const float32* q2 = quaternion.Values;
	float32 dotProduct = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];

	if (dotProduct <  0.0f)
	{
		quaternion = -quaternion;
		dotProduct = -dotProduct;
	}

	if (dotProduct < 1.0001f && dotProduct > 0.9999f)
	{
		return LERP(quaternion, interpolation);
	}

	float32 angleBetweenQuaternions = acosf(dotProduct);

	Quaternion slerp((*this * sinf(angleBetweenQuaternions * (1 - interpolation)) +
		quaternion * sinf(angleBetweenQuaternions * interpolation)) /
		sinf(angleBetweenQuaternions));

	return slerp;
}

Quaternion Quaternion::LERP(Quaternion &quaternion, float32 interpolation)
{
	Quaternion lerp(((quaternion - *this) * interpolation) + *this);
	lerp.Normalize();
	return lerp;
}

void Quaternion::CalculateW()
{
	float32 t = 1.0f - (Values[0] * Values[0]) - (Values[1] * Values[1]) - (Values[2] * Values[2]);
	if( t < 0.0f )
		Values[ 3 ] = 0.0f;
	else
		Values[ 3 ] = -sqrt( t );
}

void Quaternion::Invert()
{
	Values[ 0 ] = -Values[ 0 ];
	Values[ 1 ] = -Values[ 1 ];
	Values[ 2 ] = -Values[ 2 ];
}
		
void Quaternion::RotateVector(Vector3& v) const
{
	Quaternion conj = Quaternion(*this);
	conj.Invert();

	Quaternion res = *this * Quaternion(v.X, v.Y, v.Z, 1.0f) * conj;
	v.X = res.X;
	v.Y = res.Y;
	v.Z = res.Z;
}

const Quaternion Quaternion::operator + (const Quaternion& quaternion) const
{
	return Quaternion(Values[0] + quaternion.Values[0], Values[1] + quaternion.Values[1],
		Values[2] + quaternion.Values[2], Values[3] + quaternion.Values[3]);
}

const Quaternion Quaternion::operator - (const Quaternion& quaternion) const
{
	return Quaternion(Values[0] - quaternion.Values[0], Values[1] - quaternion.Values[1],
		Values[2] - quaternion.Values[2], Values[3] - quaternion.Values[3]);
}

const Quaternion Quaternion::operator * (const Quaternion& quaternion) const
{
	Vector3 u(Values[0], Values[1], Values[2]);
	Vector3 v(quaternion.Values[0], quaternion.Values[1], quaternion.Values[2]);

	Vector3 cross = u.CrossProduct(v);
	Vector3 uv = (v * Values[3]) + (u * quaternion.Values[3]) + cross;

	return Quaternion(uv.X, uv.Y, uv.Z, (Values[3] * quaternion.Values[3]) - ((v.X * u.X) + (v.Y * u.Y) + (v.Z * u.Z)));
}

const void Quaternion::operator += (const Quaternion& quaternion)
{
	Values[0] += quaternion.Values[0];
	Values[1] += quaternion.Values[1];
	Values[2] += quaternion.Values[2];
	Values[3] += quaternion.Values[3];
}

const void Quaternion::operator -= (const Quaternion& quaternion)
{
	Values[0] -= quaternion.Values[0];
	Values[1] -= quaternion.Values[1];
	Values[2] -= quaternion.Values[2];
	Values[3] -= quaternion.Values[3];
}

const void Quaternion::operator *= (const Quaternion& quaternion)
{
	Vector3 u(Values[0], Values[1], Values[2]);
	Vector3 v(quaternion.Values[0], quaternion.Values[1], quaternion.Values[2]);

	Vector3 cross = u.CrossProduct(v);//Vector3((u.y * v.z - u.z * v.y), (u.z * v.x - u.x * v.z), (u.x * v.y - u.y * v.x));
	Vector3 uv = (v * Values[3]) + (u * quaternion.Values[3]) + cross;

	*this = Quaternion(uv.X, uv.Y, uv.Z, (Values[3] * quaternion.Values[3]) - ((v.X * u.X) + (v.Y * u.Y) + (v.Z * u.Z)));
}

const Quaternion Quaternion::operator * (float scalar) const
{
	return Quaternion(Values[0] * scalar, Values[1] * scalar, Values[2] * scalar, Values[3] * scalar);
}

const Quaternion Quaternion::operator / (float scalar) const
{
	float32 scalInv = 1.0f / scalar;
	return Quaternion(Values[0] * scalInv, Values[1] * scalInv, Values[2] * scalInv, Values[3] * scalInv);
}

const void Quaternion::operator *= (float32 scalar)
{
	Values[0] *= scalar;
	Values[1] *= scalar;
	Values[2] *= scalar;
	Values[3] *= scalar;
}

const void Quaternion::operator /= (float32 scalar)
{
	Values[0] /= scalar;
	Values[1] /= scalar;
	Values[2] /= scalar;
	Values[3] /= scalar;
}

const Quaternion Quaternion::operator * (const Vector3& vector3) const
{
	return Quaternion(-(Values[0] * vector3.X + Values[1] * vector3.Y + Values[2] * vector3.Z),
						Values[3] * vector3.X + Values[1] * vector3.Z + Values[2] * vector3.Y,
						Values[3] * vector3.Y + Values[2] * vector3.X + Values[0] * vector3.Z,
						Values[3] * vector3.Z + Values[0] * vector3.Y + Values[1] * vector3.X);
}

const void Quaternion::operator *= (const Vector3& vector3)
{
	Quaternion tempQuaternion(-(Values[0] * vector3.X + Values[1] * vector3.Y + Values[2] * vector3.Z),
								Values[3] * vector3.X + Values[1] * vector3.Z + Values[2] * vector3.Y,
								Values[3] * vector3.Y + Values[2] * vector3.X + Values[0] * vector3.Z,
								Values[3] * vector3.Z + Values[0] * vector3.Y + Values[1] * vector3.X);

	Values[0] = tempQuaternion.Values[0];
	Values[1] = tempQuaternion.Values[1];
	Values[2] = tempQuaternion.Values[2];
	Values[3] = tempQuaternion.Values[3];
}

const Quaternion Quaternion::operator - () const
{
	return Quaternion(-Values[0], -Values[1], -Values[2], -Values[3]);
}

const Quaternion Quaternion::operator ~ () const
{
	return Quaternion(-Values[0], -Values[1], -Values[2], Values[3]);
}

const void Quaternion::operator = (const Quaternion& quaternion)
{
	Values[0] = quaternion.Values[0];
	Values[1] = quaternion.Values[1];
	Values[2] = quaternion.Values[2];
	Values[3] = quaternion.Values[3];
}

bool Quaternion::operator == (const Quaternion& quaternion) 
{
	return (Values[0] == quaternion.Values[0] && Values[1] == quaternion.Values[1] &&
		Values[2] == quaternion.Values[2] && Values[3] == quaternion.Values[3]);
}

bool Quaternion::operator != (const Quaternion& quaternion)
{
	return (!(*this == quaternion));
}