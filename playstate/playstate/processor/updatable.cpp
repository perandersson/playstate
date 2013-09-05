#include "../memory/memory.h"
#include "updatable.h"
#include "update_processor.h"
using namespace playstate;

Updatable::Updatable() : mAttachedToProcessor(NULL)
{
}

Updatable::~Updatable()
{
	Detach();
}

void Updatable::Attach(IUpdateProcessor* processor)
{
	assert_not_null(processor);
	processor->AttachUpdatable(this);
	mAttachedToProcessor = processor;
}

void Updatable::Detach()
{
	if(mAttachedToProcessor != NULL) {
		mAttachedToProcessor->DetachUpdatable(this);
		mAttachedToProcessor = NULL;
	}
}
