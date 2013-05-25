#pragma once

#include "vector3.h"

namespace playstate
{
	//
	// http://en.wikipedia.org/wiki/Plane_(geometry)
	// http://www.lighthouse3d.com/tutorials/maths/plane/
	// http://www.lighthouse3d.com/tutorials/maths/
	//
	class Plane
	{
	public:
		Plane();
		Plane(const Plane& plane);
		Plane(const Vector3& v1, const Vector3& v2, const Vector3& v3);
		~Plane();

		//
		// http://www.lighthouse3d.com/tutorials/maths/plane/
		// http://mathworld.wolfram.com/Point-PlaneDistance.html
		// @remark Although not entirely true, this method returns the distance between the internal point on this plane
		//		with the supplied point. In realtity there isn't simply a "point" on the plane. This method is only used
		//		together with the frustum culling engine.
		// @return The distance between the point in the center of the plane and the supplied point.
		float32 GetDistance(const Vector3& point) const;

	public:
		void operator = (const Plane& plane);

	public:
		Vector3 Normal;
		Vector3 Point;
	};
}
