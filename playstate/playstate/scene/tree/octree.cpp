#include "../../memory/memory.h"
#include "octree.h"
using namespace playstate;

Octree::Octree(const AABB& boundingBox, uint32 level, uint32 maxLevel, Octree* top)
	: mBoundingBox(boundingBox), mMaxLevel(maxLevel), mLevel(level), mTop(top), mNodes(offsetof(SpatialNode, SpatialNodeLink))
{
	assert_not_null(top);
	Initialize(boundingBox, level, maxLevel);
}

Octree::Octree(uint32 maxLevel)
	: mBoundingBox(AABB(Vector3(0, 0, 0), 1000.0f, 1000.0f, 1000.0f)), mMaxLevel(maxLevel), mLevel(0), mTop(0), mNodes(offsetof(SpatialNode, SpatialNodeLink))
{
	mTop = this;
	Initialize(mBoundingBox, mLevel, maxLevel);
}

Octree::~Octree()
{
	Clean();
}

bool Octree::Add(SpatialNode* node)
{
	assert_not_null(node);
	Insert(node);
	return true;
}

void Octree::Remove(SpatialNode* node)
{
	assert_not_null(node);
	ISpatialTree* nodeInTree = node->GetTree();
	if(nodeInTree == this)
		mNodes.Remove(node);
	else
		nodeInTree->Remove(node);
}
		
void Octree::Find(const Frustum& frustum, ISpatialTreeVisitor* visitor) const
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
		mParts[TOP_FRONT_LEFT]->Find(frustum, visitor);
		mParts[TOP_FRONT_RIGHT]->Find(frustum, visitor);
		mParts[TOP_BACK_LEFT]->Find(frustum, visitor);
		mParts[TOP_BACK_RIGHT]->Find(frustum, visitor);
		mParts[BOTTOM_FRONT_LEFT]->Find(frustum, visitor);
		mParts[BOTTOM_FRONT_RIGHT]->Find(frustum, visitor);
		mParts[BOTTOM_BACK_LEFT]->Find(frustum, visitor);
		mParts[BOTTOM_BACK_RIGHT]->Find(frustum, visitor);
	}
	
	SpatialNode* node = mNodes.First();
	while(node != NULL) {
		SpatialNode* tmp = node->SpatialNodeLink.Tail;
		if(frustum.IsColliding(node->GetBoundingBox()) != AABB::OUTSIDE)
			visitor->Visit(node);
		node = tmp;
	}
}
		
void Octree::Find(const AABB& boundingBox, ISpatialTreeVisitor* visitor) const
{
	assert_not_null(visitor);

	AABB::CollisionResult result = mBoundingBox.IsColliding(boundingBox);
	if(result == AABB::OUTSIDE)
		return;

	if(result == AABB::CONTAINED) {
		IterateAndVisit(visitor);
		return;
	}

	if(!IsLeafNode()) {
		mParts[TOP_FRONT_LEFT]->Find(boundingBox, visitor);
		mParts[TOP_FRONT_RIGHT]->Find(boundingBox, visitor);
		mParts[TOP_BACK_LEFT]->Find(boundingBox, visitor);
		mParts[TOP_BACK_RIGHT]->Find(boundingBox, visitor);
		mParts[BOTTOM_FRONT_LEFT]->Find(boundingBox, visitor);
		mParts[BOTTOM_FRONT_RIGHT]->Find(boundingBox, visitor);
		mParts[BOTTOM_BACK_LEFT]->Find(boundingBox, visitor);
		mParts[BOTTOM_BACK_RIGHT]->Find(boundingBox, visitor);
	}

	SpatialNode* node = mNodes.First();
	while(node != NULL) {
		SpatialNode* tmp = node->SpatialNodeLink.Tail;
		if(node->GetBoundingBox().IsColliding(boundingBox) != AABB::OUTSIDE)
			visitor->Visit(node);
		node = tmp;
	}
}

void Octree::IterateAndVisit(ISpatialTreeVisitor* visitor) const
{
	SpatialNode* node = mNodes.First();
	while(node != NULL) {
		SpatialNode* tmp = node->SpatialNodeLink.Tail;
		visitor->Visit(node);
		node = tmp;
	}
	
	if(!IsLeafNode()) {
		mParts[TOP_FRONT_LEFT]->IterateAndVisit(visitor);
		mParts[TOP_FRONT_RIGHT]->IterateAndVisit(visitor);
		mParts[TOP_BACK_LEFT]->IterateAndVisit(visitor);
		mParts[TOP_BACK_RIGHT]->IterateAndVisit(visitor);
		mParts[BOTTOM_FRONT_LEFT]->IterateAndVisit(visitor);
		mParts[BOTTOM_FRONT_RIGHT]->IterateAndVisit(visitor);
		mParts[BOTTOM_BACK_LEFT]->IterateAndVisit(visitor);
		mParts[BOTTOM_BACK_RIGHT]->IterateAndVisit(visitor);
	}

}

void Octree::Clean()
{
	if(mParts[0] != NULL) {
		for(int i = 0; i < NUM_PARTS; ++i) {
			mParts[i]->Clean();
			delete mParts[i];
			mParts[i] = NULL;
		}
	}
}
	
void Octree::Invalidate(SpatialNode* node)
{
	assert_not_null(node);

	Remove(node);
	mTop->Add(node);
}

void Octree::Initialize(const AABB& boundingBox, uint32 level, uint32 maxLevel)
{
	memset(mParts, 0 , sizeof(mParts));
	if(level >= maxLevel)
		return;

	const Vector3 mid = boundingBox.GetPosition();
	const float partWidth = boundingBox.GetWidth() / 2.0f;
	const float partHeight = boundingBox.GetHeight() / 2.0f;
	const float partDepth = boundingBox.GetDepth() / 2.0f;

	const Vector3& minPoint = boundingBox.MinPoint;
	const Vector3& maxPoint = boundingBox.MaxPoint;

	// Can use TFL and BBR to calculate all midpoints for each part.
	const Vector3 tfl((minPoint.X + mid.X) / 2.0f, 
		(maxPoint.Y + mid.Y) / 2.0f, 
		(minPoint.Z + mid.Z) / 2.0f);
	const Vector3 bbr((maxPoint.X + mid.X) / 2.0f, 
		(minPoint.Y + mid.Y) / 2.0f,
		(maxPoint.Z + mid.Z) / 2.0f);

	// Create child nodes
	mParts[TOP_FRONT_LEFT] = new Octree(AABB(tfl, 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[TOP_FRONT_RIGHT] = new Octree(AABB(Vector3(bbr.X, tfl.Y, tfl.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[TOP_BACK_LEFT] = new Octree(AABB(Vector3(tfl.X, tfl.Y, bbr.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[TOP_BACK_RIGHT] = new Octree(AABB(Vector3(bbr.X, tfl.Y, bbr.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[BOTTOM_FRONT_LEFT] = new Octree(AABB(Vector3(tfl.X, bbr.Y, tfl.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[BOTTOM_FRONT_RIGHT] = new Octree(AABB(Vector3(bbr.X, bbr.Y, tfl.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[BOTTOM_BACK_LEFT] = new Octree(AABB(Vector3(tfl.X, bbr.Y, bbr.Z), 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);

	mParts[BOTTOM_BACK_RIGHT] = new Octree(AABB(bbr, 
		partWidth, partHeight, partDepth), level + 1, maxLevel, mTop);
}

bool Octree::Insert(SpatialNode* node)
{
	// Only place the node inside the inner-most octree that's fully contain it.
	// If the item is partially inside an octree then add it to it's parent octree instead,
	// this is to prevent the same node to be added more than once inside the octree.
	AABB::CollisionResult result = mBoundingBox.IsColliding(node->GetBoundingBox());
	if(result != AABB::CONTAINED)
		return false;

	if(!IsLeafNode()) {
		if(mParts[TOP_FRONT_LEFT]->Insert(node))
			return true;
		if(mParts[TOP_FRONT_RIGHT]->Insert(node))
			return true;
		if(mParts[TOP_BACK_LEFT]->Insert(node))
			return true;
		if(mParts[TOP_BACK_RIGHT]->Insert(node))
			return true;
		if(mParts[BOTTOM_FRONT_LEFT]->Insert(node))
			return true;
		if(mParts[BOTTOM_FRONT_RIGHT]->Insert(node))
			return true;
		if(mParts[BOTTOM_BACK_LEFT]->Insert(node))
			return true;
		if(mParts[BOTTOM_BACK_RIGHT]->Insert(node))
			return true;
	}

	mNodes.AddLast(node);
	node->AttachToTree(this);
	return true;
}
