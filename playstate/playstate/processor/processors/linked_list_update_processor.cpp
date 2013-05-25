#include "../../memory/memory.h"
#include "linked_list_update_processor.h"
using namespace playstate;

LinkedListUpdateProcessor::LinkedListUpdateProcessor() 
	: mTimeSinceLastTick(0.0f)
{
}

LinkedListUpdateProcessor::~LinkedListUpdateProcessor()
{
	mUpdatables.DeleteAll();
}

void LinkedListUpdateProcessor::AttachUpdatable(IUpdatable* updatable)
{
	mUpdatables.AddLast(updatable);
}

void LinkedListUpdateProcessor::DetachUpdatable(IUpdatable* updatable)
{
	mUpdatables.Remove(updatable);
}

void LinkedListUpdateProcessor::Update()
{
	IUpdatable* updatable = mUpdatables.First();
	while(updatable != NULL) {
		IUpdatable* tmp = updatable->UpdatableLink.Tail;
		updatable->Update();
		updatable = tmp;
	}

	mTimeSinceLastTick += GameDeltaTime;
	if(mTimeSinceLastTick > SecondsPerTick) {
	/*	Tickable* tickable = mTickables.First();
		while(tickable != NULL) {
			Tickable* tmp = tickable->Link.Tail;
			tickable->UpdateTick();
			tickable = tmp;
		}*/
		mTimeSinceLastTick -= SecondsPerTick;
	}
}

LinkedListUpdateProcessorFactory::LinkedListUpdateProcessorFactory()
{
}

LinkedListUpdateProcessorFactory::~LinkedListUpdateProcessorFactory()
{
}

IUpdateProcessor* LinkedListUpdateProcessorFactory::Create() const
{
	return new LinkedListUpdateProcessor();
}
