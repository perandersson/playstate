#pragma once

#include "../collision/aabb.h"
#include "../camera/frustum.h"
#include "spatial_node.h"

namespace playstate
{
	class ISpatialTreeVisitor
	{
	public:
		//
		// Method invoked when a spatial node has been found in the tree
		//
		// @param node The found node.
		virtual void Visit(SpatialNode* node) = 0;
	};

	class ISpatialTree
	{
	public:
		virtual ~ISpatialTree() {}

	public:
		//
		// Add a new spatial node to this tree
		//
		// @param node the node
		virtual bool Add(SpatialNode* node) = 0;

		//
		// Remove the spatial node from this tree
		//
		// @param node the node we want to remove from this tree.
		virtual void Remove(SpatialNode* node) = 0;

		//
		// Invalidates the supplied node. 
		//
		// @remark This usually means that the bounding-box has changed somehow - i.e. it needs 
		//	to be moved inside the tree structure.
		//
		// @param node The invalidated node
		virtual void Invalidate(SpatialNode* node) = 0;

		//
		// Search for nodes based on the supplied frustum. The visitor will be called for each found item.
		//
		// @param frustum
		// @param visitor
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const = 0;
		//
		// Search for nodes based on the supplied boudingBox. The visitor will be called for each found item.
		//
		// @param boudingBox
		// @param visitor
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const = 0;
	};
}
