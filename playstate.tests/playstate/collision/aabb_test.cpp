#include <playstate/collision/aabb.h>
#include "../test.h"
using namespace playstate;

TestCase(AABB_DefaultConstructor, "AABB()")
{
	const AABB val;

	AssertEquals(val.MinPoint, Vector3::Zero);
	AssertEquals(val.MaxPoint, Vector3::Zero);
}

TestCase(AABB_IsNotCollidingWithBox, "val1 IsColliding val2 == AABB::OUTSIDE")
{
	const AABB val1(Vector3::Zero, 1.0f, 1.0f, 1.0f);
	const AABB val2(Vector3(1.01f, 1.01f, 1.01f), 1.0f, 1.0f, 1.0f);

	AssertEquals(val1.IsColliding(val2), AABB::OUTSIDE);
}

TestCase(AABB_IsIntersectingWithBox, "val1 IsColliding val2 == AABB::INTERSECT")
{
	const AABB val1(Vector3::Zero, 1.0f, 1.0f, 1.0f);
	const AABB val2(Vector3(0.05f, 0.5f, 0.5f), 1.0f, 1.0f, 1.0f);

	AssertEquals(val1.IsColliding(val2), AABB::INTERSECT);
}

TestCase(AABB_IsContainInsideBox, "val1 IsColliding val2 == AABB::CONTAINED")
{
	const AABB val1(Vector3(2.0f, 1.0f, 1.0f), 1.0f, 1.0f, 1.0f);
	const AABB val2(Vector3(2.0f, 1.0f, 1.0f), 0.2f, 0.2f, 0.2f);

	AssertEquals(val1.IsColliding(val2), AABB::CONTAINED);
}
