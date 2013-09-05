#include "../../memory/memory.h"
#include "octree_light_source_processor.h"
#include "../../types.h"
using namespace playstate;

OctreeLightSourceProcessor::OctreeLightSourceProcessor() : mOctree(3), mLightSources(offsetof(LightSource, LightSourceLink))
{
}

OctreeLightSourceProcessor::~OctreeLightSourceProcessor()
{
	mLightSources.DeleteAll();
}

void OctreeLightSourceProcessor::AttachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);

	mLightSources.AddLast(lightSource);
	mOctree.Add(lightSource);
}

void OctreeLightSourceProcessor::DetachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);

	mLightSources.Remove(lightSource);
	mOctree.Remove(lightSource);
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

bool OctreeLightSourceProcessor::Find(const FindQuery& query, LightSourceResultSet* target) const
{
	LightSourceEventHandlerVisitor visitor(target);
	mOctree.Find(query.Camera->GetViewFrustum(), &visitor);
	return visitor.HasFoundResults();
}
