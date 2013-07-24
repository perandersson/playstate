#include "../../memory/memory.h"
#include "octree_light_source_processor.h"
#include "../../types.h"
using namespace playstate;

// TODO: Make sure that the octree's bounding box is not a static value.
OctreeLightSourceProcessor::OctreeLightSourceProcessor() : mOctree(AABB(Vector3(0, 0, 0), 1000.0f, 1000.0f, 1000.0f), 0, 4)
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
