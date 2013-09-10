#include "../../memory/memory.h"
#include "quadtree_node.h"
using namespace playstate;

QuadTreeNode::QuadTreeNode(const AABB& boundingBox, uint32 maxDepth)
	: mParent(NULL), mChildren(offsetof(SpatialNode, SpatialNodeLink)), mBoundingBox(boundingBox), mDepth(0), mMaxDepth(maxDepth), mAlive(false)
{
	const Vector3 mid = mBoundingBox.GetPosition();
	const float partWidth = mBoundingBox.GetWidth() / 2.0f;
	const float partHeight = mBoundingBox.GetHeight();
	const float partDepth = mBoundingBox.GetDepth() / 2.0f;

	AABB topLeft(mid + Vector3(-(partWidth / 2.0f), 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[TOP_LEFT] = new QuadTreeNode(this, topLeft, 1, maxDepth);
	
	AABB topRight(mid + Vector3(partWidth / 2.0f, 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[TOP_RIGHT] = new QuadTreeNode(this, topRight, 1, maxDepth);
	
	AABB bottomLeft(mid + Vector3(-(partWidth / 2.0f), 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_LEFT] = new QuadTreeNode(this, bottomLeft, 1, maxDepth);
	
	AABB bottomRight(mid + Vector3(partWidth / 2.0f, 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_RIGHT] = new QuadTreeNode(this, bottomRight, 1, maxDepth);
}

QuadTreeNode::QuadTreeNode(QuadTreeNode* parent, const AABB& boundingBox, uint32 depth, uint32 maxDepth)
	: mParent(parent), mChildren(offsetof(SpatialNode, SpatialNodeLink)), mBoundingBox(boundingBox),
	mDepth(depth), mMaxDepth(maxDepth), mAlive(false)
{
	memset(mCorners, 0, sizeof(mCorners));
}

QuadTreeNode::~QuadTreeNode()
{
	if(mCorners[TOP_LEFT] != NULL) {
		delete mCorners[TOP_LEFT];
	}
	if(mCorners[TOP_RIGHT] != NULL) {
		delete mCorners[TOP_RIGHT];
	}
	if(mCorners[BOTTOM_LEFT] != NULL) {
		delete mCorners[BOTTOM_LEFT];
	}
	if(mCorners[BOTTOM_RIGHT] != NULL) {
		delete mCorners[BOTTOM_RIGHT];
	}

	mParent = NULL;
	mAlive = false;

	mChildren.UnlinkAll();
}

bool QuadTreeNode::Add(SpatialNode* node)
{
	// Ignore nodes not completely inside this quadtree node's bounding box.
	AABB::CollisionResult result = mBoundingBox.IsColliding(node->GetBoundingBox());
	if(result != AABB::CONTAINED) {
		if(IsRootNode()) {
			AddToThisNode(node);
			return true;
		}
		return false;
	}

	if(!IsLeafNode()) {
		// Try to add the node in one of the corners of this node. If the node
		// is overlapping multiple corners then add the node in the node above this one.
		//
		if(mCorners[TOP_LEFT]->Add(node))
			return true;
		if(mCorners[TOP_RIGHT]->Add(node))
			return true;
		if(mCorners[BOTTOM_LEFT]->Add(node))
			return true;
		if(mCorners[BOTTOM_RIGHT]->Add(node))
			return true;
	}

	AddToThisNode(node);
	return true;
}

void QuadTreeNode::MoveChildrenDown()
{
	SpatialNode* node = mChildren.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		if(mCorners[TOP_LEFT]->Add(node)) {
		} else if(mCorners[TOP_RIGHT]->Add(node)) {
		} else if(mCorners[BOTTOM_LEFT]->Add(node)) {
		} else if(mCorners[BOTTOM_RIGHT]->Add(node)) {
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
	mCorners[TOP_LEFT] = new QuadTreeNode(this, topLeft, mDepth + 1, mMaxDepth);
	
	AABB topRight(mid + Vector3(partWidth / 2.0f, 0, -(partDepth / 2.0f)), partWidth, partHeight, partDepth);
	mCorners[TOP_RIGHT] = new QuadTreeNode(this, topRight, mDepth + 1, mMaxDepth);
	
	AABB bottomLeft(mid + Vector3(-(partWidth / 2.0f), 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_LEFT] = new QuadTreeNode(this, bottomLeft, mDepth + 1, mMaxDepth);
	
	AABB bottomRight(mid + Vector3(partWidth / 2.0f, 0, partDepth / 2.0f), partWidth, partHeight, partDepth);
	mCorners[BOTTOM_RIGHT] = new QuadTreeNode(this, bottomRight, mDepth + 1, mMaxDepth);
}

bool QuadTreeNode::ResizeRequired() const
{
	if(AtMaxDepth())
		return false;

	const uint32 numChildren = mChildren.GetSize();
	return numChildren >= SizeUntilSplit;
}

void QuadTreeNode::Remove(SpatialNode* node)
{	
	// Remove the node
	mChildren.Remove(node);
	node->DetachFromTree();

	// If the number of children is small enough then make sure that
	// this items children are absorbed by it's parent
	const uint32 numChildren = CountChildren();
	if(numChildren <= SizeUntilAbsorb) {
		Suspend();
	}
}

void QuadTreeNode::Suspend()
{
	if(!IsLeafNode()) {
		if(mCorners[TOP_LEFT]->IsAlive()) 
			mCorners[TOP_LEFT]->Suspend();
		if(mCorners[TOP_RIGHT]->IsAlive()) 
			mCorners[TOP_RIGHT]->Suspend();
		if(mCorners[BOTTOM_LEFT]->IsAlive()) 
			mCorners[BOTTOM_LEFT]->Suspend();
		if(mCorners[BOTTOM_RIGHT]->IsAlive()) 
			mCorners[BOTTOM_RIGHT]->Suspend();
	}
	
	if(mParent != NULL)
		mParent->Absorb(mChildren);

	mAlive = false;
}

void QuadTreeNode::Absorb(LinkedList<SpatialNode>& children)
{
	SpatialNode* node = children.First();
	while(node != NULL) {
		SpatialNode* next = node->SpatialNodeLink.Tail;
		mChildren.AddLast(node);
		node->AttachToTree(this);
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
	size += mCorners[TOP_LEFT]->CountChildren();
	size += mCorners[TOP_RIGHT]->CountChildren();
	size += mCorners[BOTTOM_LEFT]->CountChildren();
	size += mCorners[BOTTOM_RIGHT]->CountChildren();
	return size;
}

void QuadTreeNode::AddToRoot(SpatialNode* node)
{
	// Is the node still inside this QuadTreeNode?
	AABB::CollisionResult result = mBoundingBox.IsColliding(node->GetBoundingBox());
	if(result == AABB::CONTAINED || IsRootNode()) {
		AddToThisNode(node);
		return;
	}

	mParent->AddToRoot(node);
}

void QuadTreeNode::AddToThisNode(SpatialNode* node)
{
	mChildren.AddLast(node);
	node->AttachToTree(this);
	mAlive = true;

	if(ResizeRequired()) {
		if(IsLeafNode())
			CreateCorners();
		MoveChildrenDown();		
	}
}

void QuadTreeNode::Invalidate(SpatialNode* node)
{
	assert_not_null(node);

	AABB::CollisionResult result = mBoundingBox.IsColliding(node->GetBoundingBox());
	if(result == AABB::CONTAINED || IsRootNode()) {
		// Do nothing since the the node is already inside this quadtree node.
		// Not necessary to resize the node since the resize wasn't needed before the node was invalidated.
		// Root node takes everything, even non-contained nodes.
		return;
	}

	Remove(node);
	mParent->AddToRoot(node);
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
		mCorners[TOP_LEFT]->Find(frustum, visitor);
		mCorners[TOP_RIGHT]->Find(frustum, visitor);
		mCorners[BOTTOM_LEFT]->Find(frustum, visitor);
		mCorners[BOTTOM_RIGHT]->Find(frustum, visitor);
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
		mCorners[TOP_LEFT]->IterateAndVisit(visitor);
		mCorners[TOP_RIGHT]->IterateAndVisit(visitor);
		mCorners[BOTTOM_LEFT]->IterateAndVisit(visitor);
		mCorners[BOTTOM_RIGHT]->IterateAndVisit(visitor);
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
