#pragma once

#include "../math/vector3.h"

namespace playstate
{
	//
	// Collision box based on the Axis Aligned Bounding Box principle. It's capable of determinating 
	// intersection, containing and overlapping of another AABB.
	class AABB
	{
	public:

		enum CollisionResult {
			OUTSIDE,
			INTERSECT,
			CONTAINED
		};

	public:
		AABB();
		AABB(const Vector3& position, float32 width, float32 height, float32 depth);
		AABB(const AABB& box);
		~AABB();

		//
		// @param position The new position
		void SetPosition(const Vector3& position);
		
		//
		// Translates this bounding box in the supplied direction
		void Translate(const Vector3& direction);

		//
		// @param width The new width.
		void SetWidth(float32 width);

		//
		// @param height The new hight
		void SetHeight(float32 height);

		//
		// @param depth The new depth
		void SetDepth(float32 depth);

		//
		// Checks if this bounding box is colliding with another bounding box.
		// @param aabb the other bounding box
		// @return If this method returns NOT_COLLIDING then the supplied bounding box doesn't collide with the supplied bounding box at all.
		//	INTERSECTS if the supplied bounding box overlapps but isn't fully contained by this bounding box
		//	CONTAINED if the supplied bounding box is contained fully by this bounding box.
		CollisionResult IsColliding(const AABB& otherBox) const;

		//
		// Calculates and returns the position
		Vector3 GetPosition() const;
		
		//
		// Calculates and returns the width
		float32 GetWidth() const;
		
		//
		// Calculates and returns the height
		float32 GetHeight() const;
		
		//
		// Calculates and returns the depth		
		float32 GetDepth() const;

	public:
		AABB& operator = (const AABB& box);

	private:
		void Set(const Vector3& position, float32 width, float32 height, float32 depth); 

	public:
		Vector3 MaxPoint;
		Vector3 MinPoint;

	public:
		static AABB OutsideWorld;
	};

	// TODO: Place all AABB objects in the same memory area - thus lessen the cache miss ratio on the CPU

}
