#include "../memory/memory.h"
#include "aabb.h"
#include <cassert>

using namespace playstate;

playstate::AABB playstate::AABB::OutsideWorld(playstate::Vector3(-10000.0f, -10000.0f, -10000.0f), 0.01f, 0.01f, 0.01f);

AABB::AABB()
{
}

AABB::AABB(const Vector3& position, float32 width, float32 height, float32 depth)
{
	Set(position, width, height, depth);
}

AABB::AABB(const AABB& box)
	: mMinPoint(box.mMinPoint), mMaxPoint(box.mMaxPoint)
{
}

AABB::~AABB()
{
}

void AABB::SetPosition(const Vector3& position)
{
	float32 width = mMaxPoint.X - mMinPoint.X;
	float32 height = mMaxPoint.Y - mMinPoint.Y;
	float32 depth =  mMaxPoint.Z - mMinPoint.Z;

	Set(position, width, height, depth);
}

void AABB::SetWidth(float32 width)
{
	float32 height = mMaxPoint.Y - mMinPoint.Y;
	float32 depth =  mMaxPoint.Z - mMinPoint.Z;
	
	Set(GetPosition(), width, height, depth);
}

void AABB::SetHeight(float32 height)
{
	float32 width = mMaxPoint.X - mMinPoint.X;
	float32 depth =  mMaxPoint.Z - mMinPoint.Z;
	
	Set(GetPosition(), width, height, depth);
}

void AABB::SetDepth(float32 depth)
{
	float32 width = mMaxPoint.X - mMinPoint.X;
	float32 height = mMaxPoint.Y - mMinPoint.Y;
	
	Set(GetPosition(), width, height, depth);
}

Vector3 AABB::GetPosition() const
{
	return ((mMaxPoint + mMinPoint) / 2.0f);
}
		
float32 AABB::GetWidth() const
{
	return mMaxPoint.X - mMinPoint.X;
}
		
float32 AABB::GetHeight() const
{
	return mMaxPoint.Y - mMinPoint.Y;
}
		
float32 AABB::GetDepth() const
{
	return mMaxPoint.Z - mMinPoint.Z;
}

AABB::CollisionResult AABB::IsColliding(const AABB& otherBox) const
{
	const Vector3& otherMin = otherBox.mMinPoint;
	const Vector3& otherMax = otherBox.mMaxPoint;

	// The chance of NOT colliding is bigger than either contains or intersects.

    if(mMaxPoint.X < otherMin.X || mMinPoint.X > otherMax.X)
		return OUTSIDE;

    if(mMaxPoint.Y < otherMin.Y || mMinPoint.Y > otherMax.Y)
		return OUTSIDE;
	
	if(mMaxPoint.Z < otherMin.Z || mMinPoint.Z > otherMax.Z)
		return OUTSIDE;
	
	if(otherMin.X >= mMinPoint.X && otherMax.X <= mMaxPoint.X &&
		otherMin.Y >= mMinPoint.Y && otherMax.Y <= mMaxPoint.Y &&
		otherMin.Z >= mMinPoint.Z && otherMax.Z <= mMaxPoint.Z) {
			return CONTAINED;
	}

	return INTERSECT;
}

const Vector3& AABB::GetMaxPoint() const
{
	return mMaxPoint;
}

const Vector3& AABB::GetMinPoint() const
{
	return mMinPoint;
}

void AABB::Set(const Vector3& position, float32 width, float32 height, float32 depth)
{
	assert(width > 0.0f && "You are not allowed to use negative value for the bounding box width");
	assert(height > 0.0f && "You are not allowed to use negative value for the bounding box height");
	assert(depth > 0.0f && "You are not allowed to use negative value for the bounding box depth");

	float32 halfWidth = width * 0.5f;
	float32 halfHeight = height * 0.5f;
	float32 halfDepth = depth * 0.5f;

	mMaxPoint = position;
	mMaxPoint.X += halfWidth;
	mMaxPoint.Y += halfHeight;
	mMaxPoint.Z += halfDepth;
	
	mMinPoint = position;
	mMinPoint.X -= halfWidth;
	mMinPoint.Y -= halfHeight;
	mMinPoint.Z -= halfDepth;
}

void AABB::operator = (const AABB& box)
{
	mMaxPoint = box.mMaxPoint;
	mMinPoint = box.mMinPoint;

#ifdef _DEBUG
	float32 tmp = mMaxPoint.Y - box.mMaxPoint.Y;
	assert(tmp != 10000.0f);
#endif
}
