#pragma once
#include "../update_processor.h"

namespace playstate
{
	//
	// 
	class LinkedListUpdateProcessor : public IUpdateProcessor
	{
	public:
		//
		// Constructor
		LinkedListUpdateProcessor();

		//
		// Destructor
		virtual ~LinkedListUpdateProcessor();

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* updatable);
		virtual void DetachTickable(ITickable* updatable);
		
		
	// IUpdatable
	public:
		virtual void Update();

	private:		
		LinkedList<IUpdatable> mUpdatables;
		LinkedList<ITickable> mTickables;
		float32 mTimeSinceLastTick;
	};
}
