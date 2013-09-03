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
		
	protected:
		//
		// Sets this nodes absolute position. Note that the boundingBox might be initially offsetted, which is taken
		// into account when moving this node.
		void SetPosition(const Vector3& position);

		//
		// Sets this nodes absolute rotation
		void SetRotation(const Vector3& rotation);

	private:
		ISpatialTree* mTree;
		AABB mBoundingBox;
		Vector3 mOrigin;
	};
}

