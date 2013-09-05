#include "../../memory/memory.h"
#include "quadtree_light_source_processor.h"
#include "../../types.h"
using namespace playstate;

QuadTreeLightSourceProcessor::QuadTreeLightSourceProcessor() 
	: mQuadTree(AABB(Vector3(500.f, 0, 500.f), 1100.0f, 1100.0f, 1100.0f), 3), mLightSources(offsetof(LightSource, LightSourceLink))
{
}

QuadTreeLightSourceProcessor::~QuadTreeLightSourceProcessor()
{
	mLightSources.DeleteAll();
}

void QuadTreeLightSourceProcessor::AttachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);

	mLightSources.AddLast(lightSource);
	mQuadTree.Add(lightSource);
}

void QuadTreeLightSourceProcessor::DetachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);

	mLightSources.Remove(lightSource);
	mQuadTree.Remove(lightSource);
}

class LightSourceEventHandlerVisitor : public ISpatialTreeVisitor
{
public:
	LightSourceEventHandlerVisitor(LightSourceResultSet* target)
		: mResultSetTarget(target), mFoundResults(false)
	{
		assert_not_null(target);
	}
	
	~LightSourceEventHandlerVisitor() {}

	bool HasFoundResults() const {
		return mFoundResults;
	}

// IOctreeVisitor
public:
	virtual void Visit(SpatialNode* item)
	{
		mResultSetTarget->AddResult(static_cast<LightSource*>(item));
		mFoundResults = true;
	}

private:
	LightSourceResultSet* mResultSetTarget;
	bool mFoundResults;
};

bool QuadTreeLightSourceProcessor::Find(const FindQuery& query, LightSourceResultSet* target) const
{
	LightSourceEventHandlerVisitor visitor(target);
	mQuadTree.Find(query.Camera->GetViewFrustum(), &visitor);
	return visitor.HasFoundResults();
}