#include "../../memory/memory.h"
#include "octree_light_source_processor.h"
#include "../../types.h"
using namespace playstate;

OctreeLightSourceProcessor::OctreeLightSourceProcessor() : mOctree(4)
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

class LightSourceEventHandlerVisitor : public IOctreeVisitor
{
public:
	LightSourceEventHandlerVisitor(LightSourceResultSet* target)
		: mResultSetTarget(target)
	{
		assert_not_null(target);
	}
	
	~LightSourceEventHandlerVisitor() {}

// IOctreeVisitor
public:
	virtual void Visit(OctreeNode* item)
	{
		LightSource** ptr = mResultSetTarget->GetOrCreate();
		*ptr = static_cast<LightSource*>(item);
	}

private:
	LightSourceResultSet* mResultSetTarget;
};

bool OctreeLightSourceProcessor::Find(const FindQuery& query, LightSourceResultSet* target) const
{
	LightSourceEventHandlerVisitor visitor(target);
	mOctree.FindItems(query.Camera->ViewFrustum, &visitor);
	return target->Size > 0;
}

///////////////////////////////////


OctreeLightSourceProcessorFactory::OctreeLightSourceProcessorFactory()
{
}

OctreeLightSourceProcessorFactory::~OctreeLightSourceProcessorFactory()
{
}

ILightSourceProcessor* OctreeLightSourceProcessorFactory::Create() const
{
	return new OctreeLightSourceProcessor();
}
