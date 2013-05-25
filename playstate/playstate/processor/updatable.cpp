#include "../memory/memory.h"
#include "updatable.h"
#include "update_processor_factory.h"
using namespace playstate;

Updatable::Updatable()
{
}

Updatable::~Updatable()
{
}

void Updatable::Attach(IUpdateProcessor* processor)
{
	assert_not_null(processor);

	processor->AttachUpdatable(this);
}

void Updatable::Detach()
{
	UpdatableLink.Unlink();
}
