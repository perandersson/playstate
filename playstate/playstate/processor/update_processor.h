#pragma once
#include "../singleton.h"
#include "../component/updatable.h"
#include "../component/tickable.h"

namespace playstate
{
	//
	// Interface that defines how updatable instances registers itself as update notification receivers.
	class IUpdateProcessor
	{
	public:
		virtual ~IUpdateProcessor() {}

	public:
		//
		// Attaches an updatable object to this processor. This enables it to receive update events during this
		//	applications execution time.
		//
		// @param updatable
		virtual void AttachUpdatable(IUpdatable* updatable) = 0;

		//
		// Detaches this updatable object from this processor. This disables it from receiving update events.
		//
		// @param updatable
		virtual void DetachUpdatable(IUpdatable* updatable) = 0;

		//
		// Attaches an tickable object to this processor. This enables it to receive tick events during this
		//	applications execution time.
		//
		// @param tickable
		virtual void AttachTickable(ITickable* tickable) = 0;

		//
		// Detaches this tickable object from this processor. This disables it from receiving tick events.
		//
		// @param tickable
		virtual void DetachTickable(ITickable* tickable) = 0;

		//
		// Updates this processor.
		virtual void Update() = 0;
	};
}
