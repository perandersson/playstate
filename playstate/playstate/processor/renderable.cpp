#include "../memory/memory.h"
#include "renderable.h"
#include "render_processor_factory.h"
using namespace playstate;

Renderable::Renderable() : OctreeNode(), mAttachedToProcessor(NULL), mVisible(false)
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
	mVisible = true;
}

void Renderable::Detach()
{
	if(mAttachedToProcessor != NULL && mVisible) {
		mAttachedToProcessor->DetachRenderable(this);
	}

	mAttachedToProcessor = NULL;
	mVisible = false;
}

void Renderable::Hide()
{
	if(mAttachedToProcessor != NULL && mVisible) {
		mAttachedToProcessor->DetachRenderable(this);
		mVisible = false;
	}
}

void Renderable::Show()
{
	if(mAttachedToProcessor != NULL && !mVisible) {
		mAttachedToProcessor->AttachRenderable(this);
		mVisible = true;
	}
}
