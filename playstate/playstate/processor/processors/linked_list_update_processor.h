#pragma once
#include "../update_processor_factory.h"

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
		
	// IUpdatable
	public:
		virtual void Update();

	private:		
		LinkedList<IUpdatable, &IUpdatable::UpdatableLink> mUpdatables;
		float32 mTimeSinceLastTick;
	};

	//
	// Factory for creating the linked list update processor
	class LinkedListUpdateProcessorFactory : public IUpdateProcessorFactory
	{
	public:
		LinkedListUpdateProcessorFactory();
		virtual ~LinkedListUpdateProcessorFactory();

	// IUpdateProcessorFactory
	public:
		virtual IUpdateProcessor* Create() const;
	};
}
