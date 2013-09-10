#include "../memory/memory.h"
#include "spatial_node.h"
#include "spatial_tree.h"
using namespace playstate;

SpatialNode::SpatialNode() : mBoundingBox(AABB::Unit), mTree(NULL)
{
}

SpatialNode::~SpatialNode()
{
}

void SpatialNode::SetBoundingBox(const AABB& boundingBox)
{
	mBoundingBox = boundingBox;
	mOrigin = boundingBox.GetPosition();
	if(mTree != NULL)
		mTree->Invalidate(this);
}

void SpatialNode::SetBoundingBox(const AABB& boundingBox, const Vector3& position)
{
	mBoundingBox = boundingBox;
	mOrigin = boundingBox.GetPosition();
	mBoundingBox.Translate(position);
	if(mTree != NULL)
		mTree->Invalidate(this);
}

void SpatialNode::AttachToTree(ISpatialTree* tree)
{
	mTree = tree;
}

void SpatialNode::DetachFromTree()
{
	mTree = NULL;
}

void SpatialNode::SetPosition(const Vector3& position)
{
	mBoundingBox.SetPosition(mOrigin + position);
	if(mTree != NULL)
		mTree->Invalidate(this);
}

void SpatialNode::SetRotation(const Vector3& rotation)
{
	assert_not_implemented();
}
