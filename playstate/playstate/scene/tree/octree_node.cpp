#include "../../memory/memory.h"
#include "octree_node.h"
#include "octree.h"
using namespace playstate;

OctreeNode::OctreeNode() : mBoundingBox(AABB::OUTSIDE_WORLD), mOwner(NULL),
	BoundingBox(mBoundingBox)
{
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::SetBoundingBox(const AABB& boundingBox)
{
	mBoundingBox = boundingBox;
	if(mOwner != NULL)
		mOwner->Invalidate(this);
}

void OctreeNode::SetBoundingBox(const AABB& boundingBox, const Vector3& position)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPosition(position);
	if(mOwner != NULL)
		mOwner->Invalidate(this);
}

void OctreeNode::AttachToOctree(Octree* octree)
{
	mOwner = octree;
}