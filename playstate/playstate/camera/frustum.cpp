#include "../memory/memory.h"
#include "frustum.h"
#include <cassert>
#include <cmath>

using namespace playstate;

Frustum::Frustum()
	: mNearPlane(FLT_MIN), mFarPlane(FLT_MIN), mFov(FLT_MIN), mRatio(FLT_MIN),
	mTang(FLT_MIN), mNH(FLT_MIN), mNW(FLT_MIN), mFH(FLT_MIN), mFW(FLT_MIN)
{
}

Frustum::~Frustum()
{
}

void Frustum::SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio)
{
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
	mFov = fov > 170.0f ? 170.0f : fov;
	mRatio = ratio;

	assert(mNearPlane > FLT_MIN && "Invalid near plane");
	assert(mFarPlane > FLT_MIN && "Invalid near plane");
	assert(mFov > FLT_MIN && "Invalid near plane");
	assert(mRatio > FLT_MIN && "Invalid near plane");

	// compute width and height of the near and far plane sections
	mTang = (float32)tan(ANG2RAD * (double)fov * 0.5) ;
	mNH = nearPlane * mTang;
	mNW = mNH * ratio;
	mFH = farPlane  * mTang;
	mFW = mFH * ratio;
}

void Frustum::LookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
	Vector3 dir, nc, fc, X, Y, Z;
	
	// compute the Z axis of camera
	// this axis points in the opposite direction from
	// the looking direction
	Z = (eye - center).GetNormalized();

	// X axis of camera with given "up" vector and Z axis
	X = up.CrossProduct(Z).GetNormalized();

	// the real "up" vector is the cross product of Z and X
	Y = Z.CrossProduct(X);

	// compute the centers of the near and far planes
	nc = eye - Z * mNearPlane;
	fc = eye - Z * mFarPlane;

	// compute the 4 corners of the frustum on the near plane
	Vector3 ntl = nc + Y * mNH - X * mNW;
	Vector3 ntr = nc + Y * mNH + X * mNW;
	Vector3 nbl = nc - Y * mNH - X * mNW;
	Vector3 nbr = nc - Y * mNH + X * mNW;

	// compute the 4 corners of the frustum on the far plane
	Vector3 ftl = fc + Y * mFH - X * mFW;
	Vector3 ftr = fc + Y * mFH + X * mFW;
	Vector3 fbl = fc - Y * mFH - X * mFW;
	Vector3 fbr = fc - Y * mFH + X * mFW;

	// compute the six planes
	mPlanes[TOP] = Plane(ntr, ntl, ftl);
	mPlanes[BOTTOM] = Plane(nbl, nbr, fbr);
	mPlanes[LEFT] = Plane(ntl, nbl, fbl);
	mPlanes[RIGHT] = Plane(nbr, ntr, fbr);
	mPlanes[NEARP] = Plane(ntl, ntr, nbr);
	mPlanes[FARP] = Plane(ftr, ftl, fbl);
}

AABB::CollisionResult Frustum::IsColliding(const AABB& box) const
{
	// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-testing-boxes-ii/
	// Later optimizations: http://www.cescg.org/CESCG-2002/DSykoraJJelinek/index.html

	AABB::CollisionResult result = AABB::CONTAINED;
	for(int i=0; i < 6; i++) {
		const Plane& plane = mPlanes[i];
		// is the positive vertex outside?
		if (plane.GetDistance(GetPositiveVertex(box, plane.Normal)) < 0)
			return AABB::OUTSIDE;

		// is the negative vertex outside?
		else if (plane.GetDistance(GetNegativeVertex(box, plane.Normal)) < 0)
			result =  AABB::INTERSECT;
	}

	return result;
}


Vector3 Frustum::GetPositiveVertex(const AABB& boundingBox, const Vector3& normal) const
{
	Vector3 p = boundingBox.GetMinPoint();
	if (normal.X >= 0)
		p.X = boundingBox.GetMaxPoint().X;
	if (normal.Y >= 0)
		p.Y = boundingBox.GetMaxPoint().Y;
	if (normal.Z >= 0)
		p.Z = boundingBox.GetMaxPoint().Z;

	return p;
}

Vector3 Frustum::GetNegativeVertex(const AABB& boundingBox, const Vector3& normal) const
{
	Vector3 n = boundingBox.GetMaxPoint();
	if (normal.X >= 0)
		n.X = boundingBox.GetMinPoint().X;
	if (normal.Y >= 0)
		n.Y = boundingBox.GetMinPoint().Y;
	if (normal.Z >= 0)
		n.Z = boundingBox.GetMinPoint().Z;

	return n;
}
