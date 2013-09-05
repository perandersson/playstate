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

void LinkedListUpdateProcessor::AttachUpdatable(IUpdatable* updatable)
{
	mUpdatables.AddLast(updatable);
}

void LinkedListUpdateProcessor::DetachUpdatable(IUpdatable* updatable)
{
	mUpdatables.Remove(updatable);
}

void LinkedListUpdateProcessor::AttachTickable(ITickable* tickable)
{
	mTickables.AddLast(tickable);
}

void LinkedListUpdateProcessor::DetachTickable(ITickable* tickable)
{
	mTickables.Remove(tickable);
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
	static const float32 ApproxSecondsPerTick = (1.0f / (float32)TicksPerSecond);
	if(mTimeSinceLastTick > ApproxSecondsPerTick) {
		ITickable* tickable = mTickables.First();
		while(tickable != NULL) {
			ITickable* tmp = tickable->TickableLink.Tail;
			tickable->Tick();
			tickable = tmp;
		}
		SecondsPerTick = mTimeSinceLastTick;
		mTimeSinceLastTick = 0;
	}
}
