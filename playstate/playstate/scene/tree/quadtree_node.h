#pragma once

#include "../spatial_tree.h"
#include "../../linked_list.h"
#include "../../collision/aabb.h"

namespace playstate
{
	class QuadTree;
	class QuadTreeNode : public ISpatialTree
	{
	public:
		QuadTreeNode(QuadTree* tree, QuadTreeNode* parent, const AABB& boundingBox, uint32 depth, uint32 maxDepth);
		virtual ~QuadTreeNode();
		
		//
		// @return TRUE if this node is a leaf node
		inline bool IsLeafNode() const {
			return mCorners[0] == NULL || AtMaxDepth();
		}
		
		//
		// @return TRUE if this node is a branch node (i.e. not a leaf node)
		inline bool IsBranchNode() const {
			return !IsLeafNode();
		}

		//
		// @return TRUE if this node is marked as "alive". An alive node is allowed to contain spatial node children.
		// @remark The alive status is recursive, i.e. you don't need to check this nodes children as well.
		inline bool IsAlive() const {
			return mAlive;
		}

		//
		// @return TRUE if this node is located at the maximum allowed depth
		inline bool AtMaxDepth() const {
			return mDepth >= mMaxDepth;
		}

	private:
		//
		// @return TRUE if we have to many spatial node children in this quadtree node. 
		bool ResizeRequired() const;

		//
		// Create corners
		void CreateCorners();

		//
		// Count the total amount of children.
		uint32 CountChildren() const;

		//
		// Suspends this node
		void Suspend();

		//
		// Allow this node to absorb the supplied children and clears the original children list
		void Absorb(LinkedList<SpatialNode>& children);

		//
		// Try to move this nodes children down one level
		void MoveChildrenDown();
		
		//
		// Iterate over all the children with the supplied visitor
		void IterateAndVisit(ISpatialTreeVisitor* visitor) const;

	public:
		virtual bool Add(SpatialNode* node);
		virtual void Remove(SpatialNode* node);
		virtual void Invalidate(SpatialNode* node);
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const;
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const;

	private:
		QuadTree* mQuadTree;
		QuadTreeNode* mParent;
		QuadTreeNode* mCorners[4];
		LinkedList<SpatialNode> mChildren;
		AABB mBoundingBox;
		uint32 mDepth;
		uint32 mMaxDepth;
		bool mAlive;
	};
}
