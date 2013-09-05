#include <playstate/collision/aabb.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(AABB)
{
	UNIT_TEST(AABB_DefaultConstructor)
	{
		const AABB val;

		ASSERT_EQUALS(val.MinPoint, Vector3::Zero);
		ASSERT_EQUALS(val.MaxPoint, Vector3::Zero);
	}

	UNIT_TEST(AABB_IsNotCollidingWithBox)
	{
		const AABB val1(Vector3::Zero, 1.0f, 1.0f, 1.0f);
		const AABB val2(Vector3(1.01f, 1.01f, 1.01f), 1.0f, 1.0f, 1.0f);

		ASSERT_EQUALS(val1.IsColliding(val2), AABB::OUTSIDE);
	}

	UNIT_TEST(AABB_IsIntersectingWithBox)
	{
		const AABB val1(Vector3::Zero, 1.0f, 1.0f, 1.0f);
		const AABB val2(Vector3(0.05f, 0.5f, 0.5f), 1.0f, 1.0f, 1.0f);

		ASSERT_EQUALS(val1.IsColliding(val2), AABB::INTERSECT);
	}

	UNIT_TEST(AABB_IsContainInsideBox)
	{
		const AABB val1(Vector3(2.0f, 1.0f, 1.0f), 1.0f, 1.0f, 1.0f);
		const AABB val2(Vector3(2.0f, 1.0f, 1.0f), 0.2f, 0.2f, 0.2f);

		ASSERT_EQUALS(val1.IsColliding(val2), AABB::CONTAINED);
	}
}
