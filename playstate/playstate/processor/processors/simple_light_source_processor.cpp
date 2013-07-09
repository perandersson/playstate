#include "../../memory/memory.h"
#include "simple_light_source_processor.h"
#include "../../types.h"
using namespace playstate;

SimpleLightSourceProcessor::SimpleLightSourceProcessor()
{
}

SimpleLightSourceProcessor::~SimpleLightSourceProcessor()
{
	mLightSources.DeleteAll();
}

void SimpleLightSourceProcessor::AttachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.AddLast(lightSource);
}

void SimpleLightSourceProcessor::DetachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSources.Remove(lightSource);
}

///////////////////////////////////


SimpleLightSourceProcessorFactory::SimpleLightSourceProcessorFactory()
{
}

SimpleLightSourceProcessorFactory::~SimpleLightSourceProcessorFactory()
{
}

ILightSourceProcessor* SimpleLightSourceProcessorFactory::Create() const
{
	return new SimpleLightSourceProcessor();
}
