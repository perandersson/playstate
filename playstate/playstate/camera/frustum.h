#pragma once

#include "../math/vector3.h"
#include "../math/plane.h"
#include "../collision/aabb.h"

namespace playstate
{
	// http://fgiesen.wordpress.com/2010/10/17/view-frustum-culling/
	// http://blog.makingartstudios.com/?p=155
	// http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-implementation/
	//
	class Frustum
	{
		enum FrustumPlanes
		{
			TOP = 0, 
			BOTTOM, 
			LEFT,
			RIGHT, 
			NEARP, 
			FARP,

			NUM_PLANES
		};

	public:
		Frustum();
		~Frustum();

		//
		// 
		// @param viewProjection
		void LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		//
		// Sets the perspective for this frustum
		void SetPerspective(float32 nearPlane, float32 farPlane, float32 fov, float32 ratio);

		//
		// @return
		AABB::CollisionResult IsColliding(const AABB& box) const;

	private:
		Vector3 GetPositiveVertex(const AABB& boundingBox, const Vector3& normal) const;
		Vector3 GetNegativeVertex(const AABB& boundingBox, const Vector3& normal) const;
		
	private:
		Plane mPlanes[NUM_PLANES];

		float32 mNearPlane;
		float32 mFarPlane;
		float32 mFov;
		float32 mRatio;

		float32 mTang;
		float32 mNH;
		float32 mNW;
		float32 mFH;
		float32 mFW;
	};
}
