#include "../memory/memory.h"
#include "tickable.h"
#include "../processor/update_processor.h"
using namespace playstate;

Tickable::Tickable() : mAttachedToProcessor(NULL)
{
}

Tickable::~Tickable()
{
	Detach();
}

void Tickable::Attach(IUpdateProcessor* processor)
{
	assert_not_null(processor);
	processor->AttachTickable(this);
	mAttachedToProcessor = processor;
}

void Tickable::Detach()
{
	if(mAttachedToProcessor != NULL) {
		mAttachedToProcessor->DetachTickable(this);
		mAttachedToProcessor = NULL;
	}
}
