#pragma once

#include "../collision/aabb.h"
#include "../linked_list.h"

namespace playstate
{
	class ISpatialTree;

	class SpatialNode
	{
	public:
		LinkedListLink<SpatialNode> SpatialNodeLink;

	public:
		SpatialNode();
		virtual ~SpatialNode();

		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox);
		
		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position);
		
		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& scale);

		//
		// Attaches this node to the supplied tree
		void AttachToTree(ISpatialTree* tree);
		
		// 
		// @return The spatial tree this node is attached to
		inline ISpatialTree* GetTree() {
			return mTree;
		}

		//
		// @return The bounding box
		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}

		//
		// Detaches this node from the tree it's being contained inside.
		void DetachFromTree();
		
	private:
		ISpatialTree* mTree;
		AABB mBoundingBox;
	};
}

