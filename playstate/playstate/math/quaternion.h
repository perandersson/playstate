#pragma once

#include "vector3.h"

namespace playstate
{
	struct Quaternion
	{
		union
		{
			struct
			{
				float32 X;
				float32 Y;
				float32 Z;
				float32 W;
			};
			struct
			{
				Vector3 Pos;
				float32 W;
			};
			float32 Points[4];
		};

		Quaternion() : X(0), Y(0), Z(0), W(0) {}
		Quaternion(const Quaternion& q) : X(q.X), Y(q.Y), Z(q.Z), W(q.W) {}
		
		// Operators
		Vector3 operator*(const Vector3& v) const;
		Quaternion& operator=(const Quaternion& q);
	};
}
