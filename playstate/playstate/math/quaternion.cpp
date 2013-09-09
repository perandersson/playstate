#include "../memory/memory.h"
#include "quaternion.h"
using namespace playstate;

Vector3 Quaternion::operator*(const Vector3& v) const
{
	//http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	//t = 2 * cross(q.xyz, v)
	//v' = v + q.w * t + cross(q.xyz, t)

	Vector3 t = Pos.CrossProduct(v) * 2;
	return v + (t * W) + Pos.CrossProduct(t);
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
	X = q.X;
	Y = q.Y;
	Z = q.Z;
	W = q.W;
	return *this;
}