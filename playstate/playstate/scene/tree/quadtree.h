#pragma once

#include "../spatial_tree.h"
#include "../../linked_list.h"
#include "../../collision/aabb.h"

namespace playstate
{
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
		QuadTree(QuadTree* parent, const AABB& boundingBox, uint32 depth, uint32 maxDepth);
		virtual ~QuadTree();
		
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
		// @return TRUE if this node is the root node
		inline bool IsRootNode() const {
			return mParent == NULL;
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

		//
		// Try to add the node in a reversed fasion. It is assumed that invalidated nodes (i.e. a node that is moved)
		// is most likely to be placed in the QuadTreeNode it's already added in.
		void AddToRoot(SpatialNode* node);

		//
		// Add the supplied node to this node. 
		void AddToThisNode(SpatialNode* node);

	public:
		virtual bool Add(SpatialNode* node);
		virtual void Remove(SpatialNode* node);
		virtual void Invalidate(SpatialNode* node);
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const;
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const;

	private:
		QuadTree* mParent;
		QuadTree* mCorners[4];
		LinkedList<SpatialNode> mChildren;
		AABB mBoundingBox;
		uint32 mDepth;
		uint32 mMaxDepth;
		bool mAlive;
	};
}
