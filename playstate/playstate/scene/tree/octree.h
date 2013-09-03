#pragma once

#include "../../collision/aabb.h"
#include "../../camera/frustum.h"
#include "../spatial_tree.h"

namespace playstate
{
	//
	// An octree implementation of the spatial tree.
	class Octree : public ISpatialTree
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
		virtual ~Octree();

		//
		// Clean up all items
		void Clean();

	// ISpatialTree
	public:
		virtual void Add(SpatialNode* node);
		virtual void Remove(SpatialNode* node);
		virtual void Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const;
		virtual void Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const;
		virtual void Invalidate(SpatialNode* node);

	private:
		void Initialize(const AABB& boundingBox, uint32 level, uint32 maxLevel);
		bool Insert(SpatialNode* node);
		bool IsLeafNode() const;
		void IterateAndVisit(ISpatialTreeVisitor* visitor) const;

	private:
		AABB mBoundingBox;
		uint32 mMaxLevel;
		uint32 mLevel;

		Octree* mParts[NUM_PARTS];
		Octree* mTop;

	private:
		LinkedList<SpatialNode> mNodes;
	};
}
