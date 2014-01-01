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
		virtual void AttachUpdatable(Updatable* updatable);
		virtual void DetachUpdatable(Updatable* updatable);
		virtual void AttachTickable(Tickable* updatable);
		virtual void DetachTickable(Tickable* updatable);
		
		
	// IUpdatable
	public:
		virtual void Update();

	private:		
		LinkedList<Updatable> mUpdatables;
		LinkedList<Tickable> mTickables;
		float32 mTimeSinceLastTick;
	};
}
