#include "../memory/memory.h"
#include "light_source.h"
#include "light_source_processor_factory.h"
using namespace playstate;

LightSource::LightSource() : SpatialNode()
{
}

LightSource::~LightSource()
{
}

void LightSource::Attach(ILightSourceProcessor* processor)
{
	processor->AttachLightSource(this);
}

void LightSource::Detach()
{
	LightSourceLink.Unlink();
	DetachFromTree();
}
