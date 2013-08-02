#pragma once
#include "../singleton.h"
#include "updatable.h"

namespace playstate
{
	const float SecondsPerTick = 1.0f / 32.0f;

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
		// Updates this processor.
		virtual void Update() = 0;
	};

	//
	// Factory that's responsible for creating stand-alone update processors
	class IUpdateProcessorFactory : public Singleton<IUpdateProcessorFactory>
	{
	public:
		virtual ~IUpdateProcessorFactory() {}

	public:
		//
		// @return An update processor
		virtual IUpdateProcessor* Create() const = 0;
	};
}
