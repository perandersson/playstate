#pragma once

#include "../spatial_tree.h"
#include "../../linked_list.h"

namespace playstate
{
	class QuadTreeNode;

	//
	// Quadtree implementation of the spatial tree interface. The quad-tree will resize itself
	// when the number of items change within it's own nodes.
	class QuadTree : public ISpatialTree
	{
	public:
		static const uint32 SizeUntilSplit = 25U;
		static const uint32 SizeUntilAbsorb = 6U;

		// The QuadTree corners
		enum Corners {
			TOP_LEFT = 0,
			TOP_RIGHT,
			BOTTOM_LEFT,
			BOTTOM_RIGHT
		};

	public:
		QuadTree(const AABB& boundingBox, uint32 maxDepth);
		virtual ~QuadTree();

	// ISpatialTree
	public:
		virtual bool Add(SpatialNode* node);
		virtual void Remove(SpatialNode* node);
		virtual void Invalidate(SpatialNode* node);
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const;
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const;

	private:
		QuadTreeNode* mCorners[4];
		LinkedList<SpatialNode> mChildren;
		AABB mBoundingBox;
	};
}
