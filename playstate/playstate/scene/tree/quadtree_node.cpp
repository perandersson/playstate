#include "../../memory/memory.h"
#include "quadtree_node.h"
#include "quadtree.h"
using namespace playstate;

QuadTreeNode::QuadTreeNode(QuadTree* tree, QuadTreeNode* parent, const AABB& boundingBox, uint32 depth, uint32 maxDepth)
	: mQuadTree(tree), mParent(parent), mChildren(offsetof(SpatialNode, SpatialNodeLink)), mBoundingBox(boundingBox),
	mDepth(depth), mMaxDepth(maxDepth), mAlive(false)
{
	memset(mCorners, 0, sizeof(mCorners));
}

QuadTreeNode::~QuadTreeNode()
{
	if(mCorners[0] != NULL) {
		delete mCorners[0];
	}
	if(mCorners[1] != NULL) {
		delete mCorners[1];
	}
	if(mCorners[2] != NULL) {
		delete mCorners[2];
	}
	if(mCorners[3] != NULL) {
		delete mCorners[3];
	}
}

bool QuadTreeNode::Add(SpatialNode* node)
{
	// Ignore nodes not completely inside this quadtree node's bounding box.
	AABB::CollisionResult result = mBoundingBox.IsColliding(node->GetBoundingBox());
	if(result != AABB::CONTAINED)
		return false;

	if(!IsLeafNode()) {
		// Try to add the node in one of the corners of this node. If the node
		// is overlapping multiple corners then add the node in the node above this one.
		//
		if(mCorners[QuadTree::TOP_LEFT]->Add(node))
			return true;
		if(mCorners[QuadTree::TOP_RIGHT]->Add(node))
			return true;
		if(mCorners[QuadTree::BOTTOM_LEFT]->Add(node))
			return true;
		if(mCorners[QuadTree::BOTTOM_RIGHT]->Add(node))
			return true;
	}

	mAlive = true;
	mChildren.AddLast(node);

	if(ResizeRequired()) {
		if(IsLeafNode())
			CreateCorners();
		MoveChildrenDown();		
	}

	return true;
}

void QuadTreeNode::MoveChildrenDown()
{
	SpatialNode* node = mChildren.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		if(mCorners[QuadTree::TOP_LEFT]->Add(node)) {
		} else if(mCorners[QuadTree::TOP_RIGHT]->Add(node)) {
		} else if(mCorners[QuadTree::BOTTOM_LEFT]->Add(node)) {
		} else if(mCorners[QuadTree::BOTTOM_RIGHT]->Add(node)) {
		}
		node = next;
	}
}

void QuadTreeNode::CreateCorners()
{
	assert(IsLeafNode() && "You are not allowed to create corners for a branch node");
	assert(mDepth < mMaxDepth && "You are not allowed to create corners for a node at max-depth");

	const Vector3 mid = mBoundingBox.GetPosition();
	const float partWidth = mBoundingBox.GetWidth() / 2.0f;
	const float partHeight = mBoundingBox.GetHeight();
	const float partDepth = mBoundingBox.GetDepth() / 2.0f;

	AABB topLeft(mid + Vector3(-(partWidth / 2.0f), 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[QuadTree::TOP_LEFT] = new QuadTreeNode(mQuadTree, this, topLeft, mDepth + 1, mMaxDepth);
	
	AABB topRight(mid + Vector3(partWidth / 2.0f, 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[QuadTree::TOP_RIGHT] = new QuadTreeNode(mQuadTree, this, topRight, mDepth + 1, mMaxDepth);
	
	AABB bottomLeft(mid + Vector3(-(partWidth / 2.0f), 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[QuadTree::BOTTOM_LEFT] = new QuadTreeNode(mQuadTree, this, bottomLeft, mDepth + 1, mMaxDepth);
	
	AABB bottomRight(mid + Vector3(partWidth / 2.0f, 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[QuadTree::BOTTOM_RIGHT] = new QuadTreeNode(mQuadTree, this, bottomRight, mDepth + 1, mMaxDepth);
}

bool QuadTreeNode::ResizeRequired() const
{
	if(AtMaxDepth())
		return false;

	const uint32 numChildren = mChildren.GetSize();
	return numChildren >= QuadTree::SizeUntilSplit;
}

void QuadTreeNode::Remove(SpatialNode* node)
{	
	// Remove the node
	mChildren.Remove(node);

	// If the number of children is small enough then make sure that
	// this items children are absorbed by it's parent
	const uint32 numChildren = CountChildren();
	if(numChildren <= QuadTree::SizeUntilAbsorb) {
		Suspend();
	}
}

void QuadTreeNode::Suspend()
{
	if(!IsLeafNode()) {
		if(mCorners[QuadTree::TOP_LEFT]->IsAlive()) 
			mCorners[QuadTree::TOP_LEFT]->Suspend();
		if(mCorners[QuadTree::TOP_RIGHT]->IsAlive()) 
			mCorners[QuadTree::TOP_RIGHT]->Suspend();
		if(mCorners[QuadTree::BOTTOM_LEFT]->IsAlive()) 
			mCorners[QuadTree::BOTTOM_LEFT]->Suspend();
		if(mCorners[QuadTree::BOTTOM_RIGHT]->IsAlive()) 
			mCorners[QuadTree::BOTTOM_RIGHT]->Suspend();
	}

	mParent->Absorb(mChildren);
	mAlive = false;
}

void QuadTreeNode::Absorb(LinkedList<SpatialNode>& children)
{
	SpatialNode* node = children.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		mChildren.AddLast(node);
		node = next;
	}
}

uint32 QuadTreeNode::CountChildren() const
{
	if(!IsAlive())
		return 0;

	if(IsLeafNode())
		return mChildren.GetSize();

	uint32 size = mChildren.GetSize();
	size += mCorners[QuadTree::TOP_LEFT]->CountChildren();
	size += mCorners[QuadTree::TOP_RIGHT]->CountChildren();
	size += mCorners[QuadTree::BOTTOM_LEFT]->CountChildren();
	size += mCorners[QuadTree::BOTTOM_RIGHT]->CountChildren();
	return size;
}

void QuadTreeNode::Invalidate(SpatialNode* node)
{
	assert_not_null(node);

	Remove(node);
	mQuadTree->Add(node);
}

void QuadTreeNode::Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const
{
	assert_not_null(visitor);

	AABB::CollisionResult result = frustum.IsColliding(mBoundingBox);
	if(result == AABB::OUTSIDE)
		return;
		
	if(result == AABB::CONTAINED) {
		IterateAndVisit(visitor);
		return;
	}

	if(!IsLeafNode()) {		
		mCorners[QuadTree::TOP_LEFT]->Find(frustum, visitor);
		mCorners[QuadTree::TOP_RIGHT]->Find(frustum, visitor);
		mCorners[QuadTree::BOTTOM_LEFT]->Find(frustum, visitor);
		mCorners[QuadTree::BOTTOM_RIGHT]->Find(frustum, visitor);
	}
	
	SpatialNode* node = mChildren.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		if(frustum.IsColliding(node->GetBoundingBox()) != AABB::OUTSIDE)
			visitor->Visit(node);
		node = next;
	}
}

void QuadTreeNode::IterateAndVisit(ISpatialTreeVisitor* visitor) const
{
	if(!IsLeafNode()) {
		mCorners[QuadTree::TOP_LEFT]->IterateAndVisit(visitor);
		mCorners[QuadTree::TOP_RIGHT]->IterateAndVisit(visitor);
		mCorners[QuadTree::BOTTOM_LEFT]->IterateAndVisit(visitor);
		mCorners[QuadTree::BOTTOM_RIGHT]->IterateAndVisit(visitor);
	}
	
	SpatialNode* node = mChildren.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		visitor->Visit(node);
		node = next;
	}
}

void QuadTreeNode::Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const
{
	assert_not_null(visitor);
	assert_not_implemented();
}
