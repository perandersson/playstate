#pragma once
#include "../../collision/aabb.h"
#include "../../linked_list.h"

namespace playstate
{
	class Octree;

	//
	// 
	class OctreeNode
	{
	public:
		LinkedListLink<OctreeNode> OctreeLink;

	public:
		OctreeNode();
		virtual ~OctreeNode();

		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox);
		
		//
		// Sets the bounding box for this node.
		// @param boundingBox
		// @param position
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position);

		//
		// Attaches this node to an octree
		// @param octree
		void AttachToOctree(Octree* octree);

		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}

		inline Octree* GetOctree() {
			return mOwner;
		}

	private:
		AABB mBoundingBox;
		Octree* mOwner;
	};
}
