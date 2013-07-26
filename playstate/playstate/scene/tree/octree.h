#pragma once

#include "../../collision/aabb.h"
#include "../../camera/frustum.h"
#include "octree_node.h"

namespace playstate
{
	class IOctreeVisitor
	{
	public:
		//
		// Method called when an item is found inside the octree.
		// @param item The item that's found.
		virtual void Visit(OctreeNode* item) = 0;
	};

	//
	// http://en.wikipedia.org/wiki/Octree
	//
	// TODO: Add support for BSP-trees, such as indoor objects inside the Octree.
	class Octree
	{
		enum OctreeParts
		{
			TOP_FRONT_LEFT = 0,
			TOP_FRONT_RIGHT,
			TOP_BACK_LEFT,
			TOP_BACK_RIGHT,
			BOTTOM_FRONT_LEFT,
			BOTTOM_FRONT_RIGHT,
			BOTTOM_BACK_LEFT,
			BOTTOM_BACK_RIGHT,

			NUM_PARTS
		};

	protected:
		Octree(const AABB& boundingBox, uint32 level, uint32 maxLevel, Octree* top);
		
	public:
		//
		// @param maxLevel
		// @todo Make sure that the octree's bounding box is not a static value but is instead dynamic - i.e. it resizes when in need
		Octree(uint32 maxLevel);

		//
		// Destructor
		~Octree();

		//
		// Add a new renderable instance in this quad-tree
		// @param item the item 
		// @param boundingBox the objects bounding box
		void Add(OctreeNode* node);

		//
		// Remove the supplied data object from the quadtree
		// @param data the data we want to remove from the quad-tree.
		void Remove(OctreeNode* node);
		
		//
		// Search for items based on the supplied frustum. Visitor implementation will be called for each found item.
		// @param frustum
		// @param visitor
		void FindItems(const Frustum& frustum, IOctreeVisitor* visitor) const;
		
		//
		// Search for items based on the supplied boundingBox. Visitor implementation will be called for each found item.
		// @param boundingBox
		// @param visitor
		void FindItems(const AABB& boundingBox, IOctreeVisitor* visitor) const;

		//
		// Clean up all items
		void Clean();

		//
		// Invalidates a specific node. This usually means that the bounding-box has changed somehow - i.e. it needs to be moved inside the
		// octree tree structure.
		//
		// @param node 
		void Invalidate(OctreeNode* node);

	private:
		void Initialize(const AABB& boundingBox, uint32 level, uint32 maxLevel);

		//
		bool Insert(OctreeNode* node);

		//
		// Checks if this octree is a leaf tree-node.
		bool IsLeafNode() const;

		//
		// Method called if the current octree is completely inside the view - i.e. no collision detection is needed.
		// @param visitor
		void IterateAndVisit(IOctreeVisitor* visitor) const;

	private:
		AABB mBoundingBox;
		uint32 mMaxLevel;
		uint32 mLevel;

		Octree* mParts[NUM_PARTS];
		Octree* mTop;

	private:
		LinkedList<OctreeNode, &OctreeNode::OctreeLink> mNodes;
	};
}
