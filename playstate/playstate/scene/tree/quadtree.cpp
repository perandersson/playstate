#include "../../memory/memory.h"
#include "quadtree.h"
#include "quadtree_node.h"
using namespace playstate;

QuadTree::QuadTree(const AABB& boundingBox, uint32 maxDepth)
	: mChildren(offsetof(SpatialNode, SpatialNodeLink)), mBoundingBox(boundingBox)
{
	const Vector3 mid = mBoundingBox.GetPosition();
	const float partWidth = mBoundingBox.GetWidth() / 2.0f;
	const float partHeight = mBoundingBox.GetHeight();
	const float partDepth = mBoundingBox.GetDepth() / 2.0f;

	AABB topLeft(mid + Vector3(-(partWidth / 2.0f), 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[TOP_LEFT] = new QuadTreeNode(this, NULL, topLeft, 1, maxDepth);
	
	AABB topRight(mid + Vector3(partWidth / 2.0f, 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[TOP_RIGHT] = new QuadTreeNode(this, NULL, topRight, 1, maxDepth);
	
	AABB bottomLeft(mid + Vector3(-(partWidth / 2.0f), 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_LEFT] = new QuadTreeNode(this, NULL, bottomLeft, 1, maxDepth);
	
	AABB bottomRight(mid + Vector3(partWidth / 2.0f, 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_RIGHT] = new QuadTreeNode(this, NULL, bottomRight, 1, maxDepth);
}

QuadTree::~QuadTree()
{
	delete mCorners[TOP_LEFT];
	delete mCorners[TOP_RIGHT];
	delete mCorners[BOTTOM_LEFT];
	delete mCorners[BOTTOM_RIGHT];
}

bool QuadTree::Add(SpatialNode* node)
{
	assert_not_null(node);
	if(mCorners[TOP_LEFT]->Add(node))
		return true;
	if(mCorners[TOP_RIGHT]->Add(node))
		return true;
	if(mCorners[BOTTOM_LEFT]->Add(node))
		return true;
	if(mCorners[BOTTOM_RIGHT]->Add(node))
		return true;
	
	mChildren.AddLast(node);
	return true;
}

void QuadTree::Remove(SpatialNode* node)
{
	assert_not_null(node);
}

void QuadTree::Invalidate(SpatialNode* node)
{
}

void QuadTree::Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const
{
	assert_not_null(visitor);

	AABB::CollisionResult result = frustum.IsColliding(mBoundingBox);
	if(result == AABB::OUTSIDE)
		return;
		
	mCorners[QuadTree::TOP_LEFT]->Find(frustum, visitor);
	mCorners[QuadTree::TOP_RIGHT]->Find(frustum, visitor);
	mCorners[QuadTree::BOTTOM_LEFT]->Find(frustum, visitor);
	mCorners[QuadTree::BOTTOM_RIGHT]->Find(frustum, visitor);

	SpatialNode* node = mChildren.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		if(frustum.IsColliding(node->GetBoundingBox()) != AABB::OUTSIDE)
			visitor->Visit(node);
		node = next;
	}
}

void QuadTree::Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const
{
	assert_not_implemented();
}
