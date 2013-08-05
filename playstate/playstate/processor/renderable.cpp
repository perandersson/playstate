#include "../memory/memory.h"
#include "renderable.h"
#include "render_processor_factory.h"
using namespace playstate;

Renderable::Renderable() : OctreeNode(), mAttachedToProcessor(NULL)
{
}

Renderable::~Renderable()
{
	Detach();
}

void Renderable::Attach(IRenderProcessor* processor)
{
	assert_not_null(processor);
	processor->AttachRenderable(this);
	mAttachedToProcessor = processor;
}

void Renderable::Detach()
{
	if(mAttachedToProcessor != NULL) {
		mAttachedToProcessor->DetachRenderable(this);
		mAttachedToProcessor = NULL;
	}
}
