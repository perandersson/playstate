#include "../../memory/memory.h"
#include "linked_list_update_processor.h"
using namespace playstate;

float32 playstate::SecondsPerTick = 0.0f;

LinkedListUpdateProcessor::LinkedListUpdateProcessor()
	: mTimeSinceLastTick(0), mUpdatables(offsetof(Updatable, UpdatableLink)), mTickables(offsetof(Tickable, TickableLink))
{
}

LinkedListUpdateProcessor::~LinkedListUpdateProcessor()
{
	mUpdatables.DeleteAll();
	mTickables.DeleteAll();
}

void LinkedListUpdateProcessor::AttachUpdatable(Updatable* updatable)
{
	mUpdatables.AddLast(updatable);
}

void LinkedListUpdateProcessor::DetachUpdatable(Updatable* updatable)
{
	mUpdatables.Remove(updatable);
}

void LinkedListUpdateProcessor::AttachTickable(Tickable* tickable)
{
	mTickables.AddLast(tickable);
}

void LinkedListUpdateProcessor::DetachTickable(Tickable* tickable)
{
	mTickables.Remove(tickable);
}

void LinkedListUpdateProcessor::Update()
{
	Updatable* updatable = mUpdatables.First();
	while(updatable != NULL) {
		Updatable* tmp = updatable->UpdatableLink.Tail;
		updatable->Update();
		updatable = tmp;
	}

	mTimeSinceLastTick += GameDeltaTime;
	static const float32 ApproxSecondsPerTick = (1.0f / (float32)TicksPerSecond);
	if(mTimeSinceLastTick > ApproxSecondsPerTick) {
		Tickable* tickable = mTickables.First();
		while(tickable != NULL) {
			Tickable* tmp = tickable->TickableLink.Tail;
			tickable->Tick();
			tickable = tmp;
		}
		SecondsPerTick = mTimeSinceLastTick;
		mTimeSinceLastTick = 0;
	}
}
