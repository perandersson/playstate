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
	if(mTree != NULL)
		mTree->Invalidate(this);
}

void SpatialNode::SetBoundingBox(const AABB& boundingBox, const Vector3& position)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPositionRotationScale(position, Vector3::Zero, Vector3::One);
	if(mTree != NULL)
		mTree->Invalidate(this);
}

void SpatialNode::SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& scale)
{
	mBoundingBox = boundingBox;
	mBoundingBox.SetPositionRotationScale(position, Vector3::Zero, scale);
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
