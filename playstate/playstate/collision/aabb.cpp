#include "../memory/memory.h"
#include "aabb.h"
#include <cassert>

using namespace playstate;

playstate::AABB playstate::AABB::Unit(playstate::Vector3(), 1, 1, 1);

AABB::AABB()
{
}

AABB::AABB(const Vector3& position, float32 width, float32 height, float32 depth)
{
	Set(position, width, height, depth);
}

AABB::AABB(const AABB& box)
	: MinPoint(box.MinPoint), MaxPoint(box.MaxPoint)
{
}

AABB::~AABB()
{
}

void AABB::SetPosition(const Vector3& position)
{
	float32 width = MaxPoint.X - MinPoint.X;
	float32 height = MaxPoint.Y - MinPoint.Y;
	float32 depth =  MaxPoint.Z - MinPoint.Z;

	Set(position, width, height, depth);
}

void AABB::Translate(const Vector3& direction)
{
	float32 width = MaxPoint.X - MinPoint.X;
	float32 height = MaxPoint.Y - MinPoint.Y;
	float32 depth =  MaxPoint.Z - MinPoint.Z;

	Set(GetPosition() + direction, width, height, depth);
}

void AABB::SetWidth(float32 width)
{
	float32 height = MaxPoint.Y - MinPoint.Y;
	float32 depth =  MaxPoint.Z - MinPoint.Z;
	
	Set(GetPosition(), width, height, depth);
}

void AABB::SetHeight(float32 height)
{
	float32 width = MaxPoint.X - MinPoint.X;
	float32 depth =  MaxPoint.Z - MinPoint.Z;
	
	Set(GetPosition(), width, height, depth);
}

void AABB::SetDepth(float32 depth)
{
	float32 width = MaxPoint.X - MinPoint.X;
	float32 height = MaxPoint.Y - MinPoint.Y;
	
	Set(GetPosition(), width, height, depth);
}

Vector3 AABB::GetPosition() const
{
	return ((MaxPoint + MinPoint) / 2.0f);
}
		
float32 AABB::GetWidth() const
{
	return MaxPoint.X - MinPoint.X;
}
		
float32 AABB::GetHeight() const
{
	return MaxPoint.Y - MinPoint.Y;
}
		
float32 AABB::GetDepth() const
{
	return MaxPoint.Z - MinPoint.Z;
}

AABB::CollisionResult AABB::IsColliding(const AABB& otherBox) const
{
	const Vector3& otherMin = otherBox.MinPoint;
	const Vector3& otherMax = otherBox.MaxPoint;

	// The chance of NOT colliding is bigger than either contains or intersects.

    if(MaxPoint.X < otherMin.X || MinPoint.X > otherMax.X)
		return OUTSIDE;

    if(MaxPoint.Y < otherMin.Y || MinPoint.Y > otherMax.Y)
		return OUTSIDE;
	
	if(MaxPoint.Z < otherMin.Z || MinPoint.Z > otherMax.Z)
		return OUTSIDE;
	
	if(otherMin.X >= MinPoint.X && otherMax.X <= MaxPoint.X &&
		otherMin.Y >= MinPoint.Y && otherMax.Y <= MaxPoint.Y &&
		otherMin.Z >= MinPoint.Z && otherMax.Z <= MaxPoint.Z) {
			return CONTAINED;
	}

	return INTERSECT;
}

void AABB::Set(const Vector3& position, float32 width, float32 height, float32 depth)
{
	assert(width > 0.0f && "You are not allowed to use negative value for the bounding box width");
	assert(height > 0.0f && "You are not allowed to use negative value for the bounding box height");
	assert(depth > 0.0f && "You are not allowed to use negative value for the bounding box depth");

	float32 halfWidth = width * 0.5f;
	float32 halfHeight = height * 0.5f;
	float32 halfDepth = depth * 0.5f;

	MaxPoint = position;
	MaxPoint.X += halfWidth;
	MaxPoint.Y += halfHeight;
	MaxPoint.Z += halfDepth;
	
	MinPoint = position;
	MinPoint.X -= halfWidth;
	MinPoint.Y -= halfHeight;
	MinPoint.Z -= halfDepth;
}

AABB& AABB::operator = (const AABB& box)
{
	MaxPoint = box.MaxPoint;
	MinPoint = box.MinPoint;
	return *this;
}
