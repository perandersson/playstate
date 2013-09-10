#include "../memory/memory.h"
#include "light_source.h"
#include "light_source_processor.h"
using namespace playstate;

LightSource::LightSource() : SpatialNode(), mAttachedToProcessor(NULL)
{
}

LightSource::~LightSource()
{
	Detach();
}

void LightSource::Attach(ILightSourceProcessor* processor)
{
	assert_not_null(processor);
	processor->AttachLightSource(this);
	mAttachedToProcessor = processor;
}

void LightSource::Detach()
{
	if(mAttachedToProcessor != NULL) {
		mAttachedToProcessor->DetachLightSource(this);
	}

	mAttachedToProcessor = NULL;
}
