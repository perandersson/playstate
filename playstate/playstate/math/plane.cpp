#include "../memory/memory.h"
#include "plane.h"
using namespace playstate;

Plane::Plane()
{
}

Plane::Plane(const Plane& plane) 
	: Normal(plane.Normal), Point(plane.Point)
{
}

Plane::Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	const Vector3 aux1 = (v1 - v2).GetNormalized();
	const Vector3 aux2 = (v3 - v2).GetNormalized();

	Normal = aux2.CrossProduct(aux1);
	Normal.Normalize();

	Point.Set(v2);
}

Plane::~Plane()
{
}

float32 Plane::GetDistance(const Vector3& point) const
{
	Vector3 delta = point - Point;
	float32 d = Normal.DotProduct(delta);
	return d;
}

void Plane::operator = (const Plane& plane)
{
	Normal = plane.Normal;
	Point = plane.Point;
}
