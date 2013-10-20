#pragma once
#include "../math/vector3.h"
#include "../linked_list.h"

namespace playstate
{
	//
	//
	class ICollidable
	{
	public:
		virtual ~ICollidable() {}

	public:
		//
		// Method invoked when this object collides with another object
		//
		// @param otherObject
		// @param direction
		virtual void CollidesWith(ICollidable* otherObject, const Vector3& direction) = 0;
	};

	//
	//
	class Collidable : public ICollidable
	{
	public:
		LinkedListLink<Collidable> CollidableLink;

	public:
		Collidable();
		virtual ~Collidable();

		void SetCollisionGroups(uint32 groups);
		void EnableCollisionGroup(uint32 groupID);
		void DisableCollisionGroup(uint32 groupID);

		inline uint32 GetCollisionGroups() const {
			return mCollisionGroups;
		}

	private:
		uint32 mCollisionGroups;
	};
}
