#include "../memory/memory.h"
#include "matrix4x4.h"

#include <cmath>
#include <string>

using namespace playstate;

const Matrix4x4 Matrix4x4::Identity(1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1);

const Matrix4x4 Matrix4x4::Zero(0, 0, 0, 0,
								0, 0, 0, 0,
								0, 0, 0, 0,
								0, 0, 0, 0);

Matrix4x4 Matrix4x4::Translation(const Vector3& vec)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html
	return Matrix4x4(1, 0, 0, 0,
					 0, 1, 0, 0,
					 0, 0, 1, 0,
					 vec.X, vec.Y, vec.Z, 1);
}


Matrix4x4 Matrix4x4::Rotation(const Vector3& rot)
{
	Matrix4x4 result;
	if(abs(rot.X) > 0.0f) {
		result = Matrix4x4::RotationX(rot.X);
	}

	if(abs(rot.Y) > 0.0f) {
		result *= Matrix4x4::RotationY(rot.Y);
	}
	
	if(abs(rot.Z) > 0.0f) {
		result *= Matrix4x4::RotationZ(rot.Z);
	}
	result.Transpose();
	return result;
}

Matrix4x4 Matrix4x4::RotationX(float32 angleX)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleX);
	float sina = sinf(angleX);

	return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, cosa, -sina, 0.0f,
					 0.0f, sina, cosa, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationY(float32 angleY)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleY);
	float sina = sinf(angleY);

	return Matrix4x4(cosa, 0.0f, sina, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 -sina, 0.0f, cosa, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::RotationZ(float32 angleZ)
{
	// OpenGL = Transposed versions of this
	// http://www.3dcodingtutorial.com/Basic-OpenGL-functions/Translate-and-Rotate-functions.html

	float cosa = cosf(angleZ);
	float sina = sinf(angleZ);

	return Matrix4x4(cosa, -sina, 0.0f, 0.0f,
					 sina, cosa, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4x4 Matrix4x4::Scale(float32 x, float32 y, float32 z)
{
	// http://nehe.gamedev.net/article/matrices/19004/
	return Matrix4x4(x, 0, 0, 0,
					 0, y, 0, 0,
					 0, 0, z, 0,
					 0, 0, 0, 1);
}

Matrix4x4::Matrix4x4() : _11(1), _12(0), _13(0), _14(0), 
						 _21(0), _22(1), _23(0), _24(0),
						 _31(0), _32(0), _33(1), _34(0),
						 _41(0), _42(0), _43(0), _44(1)
{
}

Matrix4x4::Matrix4x4(float32 m11, float32 m12, float32 m13, float32 m14,
					 float32 m21, float32 m22, float32 m23, float32 m24,
					 float32 m31, float32 m32, float32 m33, float32 m34,
					 float32 m41, float32 m42, float32 m43, float32 m44)
{
	Values[0] = m11;
	Values[1] = m12;
	Values[2] = m13;
	Values[3] = m14;
	Values[4] = m21;
	Values[5] = m22;
	Values[6] = m23;
	Values[7] = m24;
	Values[8] = m31;
	Values[9] = m32;
	Values[10] = m33;
	Values[11] = m34;
	Values[12] = m41;
	Values[13] = m42;
	Values[14] = m43;
	Values[15] = m44;
}

Matrix4x4::Matrix4x4(const Matrix4x4 &Matrix4x4)
{
	Values[0] = Matrix4x4.Values[0];
	Values[1] = Matrix4x4.Values[1];
	Values[2] = Matrix4x4.Values[2];
	Values[3] = Matrix4x4.Values[3];
	Values[4] = Matrix4x4.Values[4];
	Values[5] = Matrix4x4.Values[5];
	Values[6] = Matrix4x4.Values[6];
	Values[7] = Matrix4x4.Values[7];
	Values[8] = Matrix4x4.Values[8];
	Values[9] = Matrix4x4.Values[9];
	Values[10] = Matrix4x4.Values[10];
	Values[11] = Matrix4x4.Values[11];
	Values[12] = Matrix4x4.Values[12];
	Values[13] = Matrix4x4.Values[13];
	Values[14] = Matrix4x4.Values[14];
	Values[15] = Matrix4x4.Values[15];
}

void Matrix4x4::Translate(const Vector3& vec)
{
    Values[3*4+0] += Values[0*4+0] * vec.X + Values[1*4+0] * vec.Y + Values[2*4+0] * vec.Z;
    Values[3*4+1] += Values[0*4+1] * vec.X + Values[1*4+1] * vec.Y + Values[2*4+1] * vec.Z;
    Values[3*4+2] += Values[0*4+2] * vec.X + Values[1*4+2] * vec.Y + Values[2*4+2] * vec.Z;
    Values[3*4+3] += Values[0*4+3] * vec.X + Values[1*4+3] * vec.Y + Values[2*4+3] * vec.Z;
}

float Matrix4x4::Determinant()
{
	float32 subMatrix0[9];
		subMatrix0[0] = Values[5]; subMatrix0[1] = Values[6]; subMatrix0[2] = Values[7];
		subMatrix0[3] = Values[9]; subMatrix0[4] = Values[10]; subMatrix0[5] = Values[11];
		subMatrix0[6] = Values[13]; subMatrix0[7] = Values[14]; subMatrix0[8] = Values[15];
	float32 subMatrix1[9];
		subMatrix1[0] = Values[4]; subMatrix1[1] = Values[6]; subMatrix1[2] = Values[7];
		subMatrix1[3] = Values[8]; subMatrix1[4] = Values[10]; subMatrix1[5] = Values[11];
		subMatrix1[6] = Values[12]; subMatrix1[7] = Values[14]; subMatrix1[8] = Values[15];
	float32 subMatrix2[9];
		subMatrix2[0] = Values[4]; subMatrix2[1] = Values[5]; subMatrix2[2] = Values[7];
		subMatrix2[3] = Values[8]; subMatrix2[4] = Values[9]; subMatrix2[5] = Values[11];
		subMatrix2[6] = Values[12]; subMatrix2[7] = Values[13]; subMatrix2[8] = Values[15];
	float32 subMatrix3[9];
		subMatrix3[0] = Values[4]; subMatrix3[1] = Values[5]; subMatrix3[2] = Values[6];
		subMatrix3[3] = Values[8]; subMatrix3[4] = Values[9]; subMatrix3[5] = Values[10];
		subMatrix3[6] = Values[12]; subMatrix3[7] = Values[13]; subMatrix3[8] = Values[14];

	float32 subDeterminant0 =subMatrix0[0] * (subMatrix0[4] * subMatrix0[8] - subMatrix0[7] * subMatrix0[5]) -
								subMatrix0[1] * (subMatrix0[3] * subMatrix0[8] - subMatrix0[6] * subMatrix0[5]) +
								subMatrix0[2] * (subMatrix0[3] * subMatrix0[7] - subMatrix0[6] * subMatrix0[4]);
	float32 subDeterminant1 =subMatrix1[0] * (subMatrix1[4] * subMatrix1[8] - subMatrix1[7] * subMatrix1[5]) -
								subMatrix1[1] * (subMatrix1[3] * subMatrix1[8] - subMatrix1[6] * subMatrix1[5]) +
								subMatrix1[2] * (subMatrix1[3] * subMatrix1[7] - subMatrix1[6] * subMatrix1[4]);
	float32 subDeterminant2 =subMatrix2[0] * (subMatrix2[4] * subMatrix2[8] - subMatrix2[7] * subMatrix2[5]) -
								subMatrix2[1] * (subMatrix2[3] * subMatrix2[8] - subMatrix2[6] * subMatrix2[5]) +
								subMatrix2[2] * (subMatrix2[3] * subMatrix2[7] - subMatrix2[6] * subMatrix2[4]);
	float32 subDeterminant3 =subMatrix3[0] * (subMatrix3[4] * subMatrix3[8] - subMatrix3[7] * subMatrix3[5]) -
								subMatrix3[1] * (subMatrix3[3] * subMatrix3[8] - subMatrix3[6] * subMatrix3[5]) +
								subMatrix3[2] * (subMatrix3[3] * subMatrix3[7] - subMatrix3[6] * subMatrix3[4]);
			
	float32 determinant = (Values[0] * subDeterminant0) - (Values[1] * subDeterminant1) + 
							 (Values[2] * subDeterminant2) - (Values[3] * subDeterminant3);

	return determinant;
}

void Matrix4x4::Transpose()
{
	float32 tempMatrix[16];
	tempMatrix[0] = Values[0];
	tempMatrix[1] = Values[1];
	tempMatrix[2] = Values[2];
	tempMatrix[3] = Values[3];
	tempMatrix[4] = Values[4];
	tempMatrix[5] = Values[5];
	tempMatrix[6] = Values[6];
	tempMatrix[7] = Values[7];
	tempMatrix[8] = Values[8];
	tempMatrix[9] = Values[9];
	tempMatrix[10] = Values[10];
	tempMatrix[11] = Values[11];
	tempMatrix[12] = Values[12];
	tempMatrix[13] = Values[13];
	tempMatrix[14] = Values[14];
	tempMatrix[15] = Values[15];

	Values[1] = tempMatrix[4];
	Values[2] = tempMatrix[8];
	Values[3] = tempMatrix[12];
	Values[4] = tempMatrix[1];
	Values[6] = tempMatrix[9];
	Values[7] = tempMatrix[13];
	Values[8] = tempMatrix[2];
	Values[9] = tempMatrix[6];
	Values[11] = tempMatrix[14];
	Values[12] = tempMatrix[3];
	Values[13] = tempMatrix[7];
	Values[14] = tempMatrix[11];
}

void Matrix4x4::Invert()
{
	unsigned int row, column, rowMax;
	float32 tmp;
	float32 *matrixA = Values;
	Matrix4x4 identity;
	float32 *matrixB = identity.Values;

	for(column = 0; column < 4; column++)
	{
		rowMax = column;
		for(row = column + 1; row < 4; row++)
		{
			if(fabs(matrixA[4 * row + column]) > fabs(matrixA[4 * rowMax + column]))
			{
				rowMax = row;
			}
		}
		if(matrixA[4 * column + rowMax] == 0.0f)
		{
			*this = Matrix4x4::Identity;
			return;
		}
		for(int c = 0; c < 4; c++)
		{
			tmp = matrixA[4 * column + c];
			matrixA[4 * column + c] = matrixA[4 * rowMax + c];
			matrixA[4 * rowMax + c] = tmp;

			tmp = matrixB[4 * column + c];
			matrixB[4 * column + c] = matrixB[4 * rowMax + c];
			matrixB[4 * rowMax + c] = tmp;
		}
		tmp = matrixA[4 * column + column];
		for(int c = 0; c < 4; c++)
		{
			matrixA[4 * column + c] /= tmp;
			matrixB[4 * column + c] /= tmp;
		}
		for(row = 0; row < 4; row++)
		{
			if(row != column)
			{
				tmp = matrixA[4 * row + column];
				for(int c = 0; c < 4; c++)
				{
					matrixA[4 * row + c] -= matrixA[4 * column + c] * tmp;
					matrixB[4 * row + c] -= matrixB[4 * column + c] * tmp;
				}
			}
		}

	}

	memcpy(Values, matrixB, sizeof(Values));
}

void Matrix4x4::AxisAndAngleToMatrix(Vector3 &vector, float32 angleRadians)
{
	const float32 cosAngle = cosf(angleRadians);
	const float32 sinAngle = sinf(angleRadians);
	const float32 t = 1 - cosAngle;

	Values[0] = t * vector.X * vector.X + cosAngle;
	Values[1] = t * vector.X * vector.Y - vector.Z * sinAngle;
	Values[2] = t * vector.X * vector.Z + vector.Y * sinAngle;
	Values[3] = 0;
			
	Values[4] = t * vector.X * vector.Y + vector.Z * sinAngle;
	Values[5] = t * vector.Y * vector.Y + cosAngle;
	Values[6] = t * vector.Y * vector.Z - vector.X * sinAngle;
	Values[7] = 0;

	Values[8] = t * vector.X * vector.Z - vector.Y * sinAngle;
	Values[9] = t * vector.Y * vector.Z + vector.X * sinAngle;
	Values[10] = t * vector.Z * vector.Z + cosAngle;
	Values[11] = 0;

	Values[12] = 0;
	Values[13] = 0;
	Values[14] = 0;
	Values[15] = 1;
}

Matrix4x4 Matrix4x4::operator + (const Matrix4x4 &matrix) const
{
	const float32 *m = Values;
	const float32 *n = matrix.Values;

	return Matrix4x4(
		m[0]  + n[0],  m[1]  + n[1],  m[2]  + n[2],  m[3]  + n[3],
		m[4]  + n[4],  m[5]  + n[5],  m[6]  + n[6],  m[7]  + n[7], 
		m[8]  + n[8],  m[9]  + n[9],  m[10] + n[10], m[11] + n[11], 
		m[12] + n[12], m[13] + n[13], m[14] + n[14], m[15] + n[15]);
}

Matrix4x4 Matrix4x4::operator - (const Matrix4x4 &matrix) const
{
	const float32 *m = Values;
	const float32 *n = matrix.Values;

	return Matrix4x4(
		m[0]  - n[0],  m[1]  - n[1],  m[2]  - n[2],  m[3]  - n[3],
		m[4]  - n[4],  m[5]  - n[5],  m[6]  - n[6],  m[7]  - n[7], 
		m[8]  - n[8],  m[9]  - n[9],  m[10] - n[10], m[11] - n[11], 
		m[12] - n[12], m[13] - n[13], m[14] - n[14], m[15] - n[15]);
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4 &matrix) const
{
	const float32 *n = Values;
	const float32 *m = matrix.Values;

	return Matrix4x4(
		(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
		(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
		(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
		(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

		(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
		(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
		(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
		(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

		(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
		(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
		(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
		(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

		(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
		(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
		(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
		(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));
}

Vector3 Matrix4x4::operator * (const Vector3& vec3) const
{
	// assume W = 1.0
	const float x = _11 * vec3.X + _12 * vec3.Y + _13 * vec3.Z + _14 * 1.0f;
	const float y = _21 * vec3.X + _22 * vec3.Y + _23 * vec3.Z + _24 * 1.0f;
	const float z = _31 * vec3.X + _32 * vec3.Y + _33 * vec3.Z + _34 * 1.0f;

	return Vector3(x, y, z);
}

const void Matrix4x4::operator += (const Matrix4x4& matrix)
{
	Values[0] += matrix.Values[0];
	Values[1] += matrix.Values[1];
	Values[2] += matrix.Values[2];
	Values[3] += matrix.Values[3];
	Values[4] += matrix.Values[4];
	Values[5] += matrix.Values[5];
	Values[6] += matrix.Values[6];
	Values[7] += matrix.Values[7];
	Values[8] += matrix.Values[8];
	Values[9] += matrix.Values[9];
	Values[10] += matrix.Values[10];
	Values[11] += matrix.Values[11];
	Values[12] += matrix.Values[12];
	Values[13] += matrix.Values[13];
	Values[14] += matrix.Values[14];
	Values[15] += matrix.Values[15];
}

const void Matrix4x4::operator -= (const Matrix4x4& matrix)
{
	Values[0] -= matrix.Values[0];
	Values[1] -= matrix.Values[1];
	Values[2] -= matrix.Values[2];
	Values[3] -= matrix.Values[3];
	Values[4] -= matrix.Values[4];
	Values[5] -= matrix.Values[5];
	Values[6] -= matrix.Values[6];
	Values[7] -= matrix.Values[7];
	Values[8] -= matrix.Values[8];
	Values[9] -= matrix.Values[9];
	Values[10] -= matrix.Values[10];
	Values[11] -= matrix.Values[11];
	Values[12] -= matrix.Values[12];
	Values[13] -= matrix.Values[13];
	Values[14] -= matrix.Values[14];
	Values[15] -= matrix.Values[15];
}

const void Matrix4x4::operator *= (const Matrix4x4& matrix)
{
	const float32 *n = Values;
	const float32 *m = matrix.Values;

	Matrix4x4 tempMatrix(
	(m[0] * n[0] + m[4] * n[1] + m[8] * n[2] + m[12] * n[3]),
	(m[1] * n[0] + m[5] * n[1] + m[9] * n[2] + m[13] * n[3]),
	(m[2] * n[0] + m[6] * n[1] + m[10] * n[2] + m[14] * n[3]),
	(m[3] * n[0] + m[7] * n[1] + m[11] * n[2] + m[15] * n[3]),

	(m[0] * n[4] + m[4] * n[5] + m[8] * n[6] + m[12] * n[7]),
	(m[1] * n[4] + m[5] * n[5] + m[9] * n[6] + m[13] * n[7]),
	(m[2] * n[4] + m[6] * n[5] + m[10] * n[6] + m[14] * n[7]),
	(m[3] * n[4] + m[7] * n[5] + m[11] * n[6] + m[15] * n[7]),

	(m[0] * n[8] + m[4] * n[9] + m[8] * n[10] + m[12] * n[11]),
	(m[1] * n[8] + m[5] * n[9] + m[9] * n[10] + m[13] * n[11]),
	(m[2] * n[8] + m[6] * n[9] + m[10] * n[10] + m[14] * n[11]),
	(m[3] * n[8] + m[7] * n[9] + m[11] * n[10] + m[15] * n[11]),

	(m[0] * n[12] + m[4] * n[13] + m[8] * n[14] + m[12] * n[15]),
	(m[1] * n[12] + m[5] * n[13] + m[9] * n[14] + m[13] * n[15]),
	(m[2] * n[12] + m[6] * n[13] + m[10] * n[14] + m[14] * n[15]),
	(m[3] * n[12] + m[7] * n[13] + m[11] * n[14] + m[15] * n[15]));

	memcpy(Values, tempMatrix.Values, sizeof(Values));
}

Matrix4x4 Matrix4x4::operator * (const float32 scalar) const
{
	return Matrix4x4(
	Values[0] * scalar, Values[1] * scalar, Values[2] * scalar, Values[3] * scalar,
	Values[4] * scalar, Values[5] * scalar, Values[6] * scalar, Values[7] * scalar,
	Values[8] * scalar, Values[9] * scalar, Values[10] * scalar, Values[11] * scalar,
	Values[12] * scalar, Values[13] * scalar, Values[14] * scalar, Values[15] * scalar);
}

Matrix4x4 Matrix4x4::operator / (const float32 scalar) const
{
	return Matrix4x4(
	Values[0] / scalar, Values[1] / scalar, Values[2] / scalar, Values[3] / scalar,
	Values[4] / scalar, Values[5] / scalar, Values[6] / scalar, Values[7] / scalar,
	Values[8] / scalar, Values[9] / scalar, Values[10] / scalar, Values[11] / scalar,
	Values[12] / scalar, Values[13] / scalar, Values[14] / scalar, Values[15] / scalar);
}

const void Matrix4x4::operator *= (const float32 scalar)
{
	Values[0] *= scalar;
	Values[1] *= scalar;
	Values[2] *= scalar;
	Values[3] *= scalar;
	Values[4] *= scalar;
	Values[5] *= scalar;
	Values[6] *= scalar;
	Values[7] *= scalar;
	Values[8] *= scalar;
	Values[9] *= scalar;
	Values[10] *= scalar;
	Values[11] *= scalar;
	Values[12] *= scalar;
	Values[13] *= scalar;
	Values[14] *= scalar;
	Values[15] *= scalar;
}

const void Matrix4x4::operator /= (const float32 scalar)
{
	Values[0] /= scalar;
	Values[1] /= scalar;
	Values[2] /= scalar;
	Values[3] /= scalar;
	Values[4] /= scalar;
	Values[5] /= scalar;
	Values[6] /= scalar;
	Values[7] /= scalar;
	Values[8] /= scalar;
	Values[9] /= scalar;
	Values[10] /= scalar;
	Values[11] /= scalar;
	Values[12] /= scalar;
	Values[13] /= scalar;
	Values[14] /= scalar;
	Values[15] /= scalar;
}

void Matrix4x4::operator = (const Matrix4x4& matrix)
{
	memcpy(Values, matrix.Values, sizeof(float32[16]));
}

bool Matrix4x4::operator == (const Matrix4x4& matrix) const
{
	if (Values[0] == matrix.Values[0] && Values[1] == matrix.Values[1] &&
		Values[2] == matrix.Values[2] && Values[3] == matrix.Values[3] &&
		Values[4] == matrix.Values[4] && Values[5] == matrix.Values[5] &&
		Values[6] == matrix.Values[6] && Values[7] == matrix.Values[7] &&
		Values[8] == matrix.Values[8] && Values[9] == matrix.Values[9] &&
		Values[10] == matrix.Values[10] && Values[11] == matrix.Values[11] &&
		Values[12] == matrix.Values[12] && Values[13] == matrix.Values[13] &&
		Values[14] == matrix.Values[14] && Values[15] == matrix.Values[15])
		return true;
	return false;
}

bool Matrix4x4::operator != (const Matrix4x4& matrix) const
{
	if (Values[0] == matrix.Values[0] && Values[1] == matrix.Values[1] &&
		Values[2] == matrix.Values[2] && Values[3] == matrix.Values[3] &&
		Values[4] == matrix.Values[4] && Values[5] == matrix.Values[5] &&
		Values[6] == matrix.Values[6] && Values[7] == matrix.Values[7] &&
		Values[8] == matrix.Values[8] && Values[9] == matrix.Values[9] &&
		Values[10] == matrix.Values[10] && Values[11] == matrix.Values[11] &&
		Values[12] == matrix.Values[12] && Values[13] == matrix.Values[13] &&
		Values[14] == matrix.Values[14] && Values[15] == matrix.Values[15])
		return false;
	return true;
}
