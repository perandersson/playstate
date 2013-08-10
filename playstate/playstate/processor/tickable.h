#pragma once

#include "../types.h"
#include "../linked_list.h"

namespace playstate
{
	class IUpdateProcessor;

	//
	// How many ticks occure every second. This is an approximation.
	// Use {@code playstate::SecondsPerTick} to see the actual amount of time in seconds has progressed since the
	// last tick.
	static const uint32 TicksPerSecond = 32;

	//
	class ITickable
	{
	public:
		LinkedListLink<ITickable> TickableLink;

	public:
		virtual ~ITickable() {}

	public:
		//
		// Method called once every tick.
		// @see {@code playstate::SecondsPerTick} for the actual amount seconds this tick is progressing.
		virtual void Tick() = 0;
	};

	//
	// Inherit this to give it's target support for tick notification
	class Tickable : public ITickable
	{
	public:
		Tickable();
		virtual ~Tickable();
		
	protected:
		//
		// Attach this tickable instance to an tickable processor. 
		// This must be done if we want this instance to receive tick events.
		// @param processor
		void Attach(IUpdateProcessor* processor);

		//
		// Detaches this instance from it's attached tickable processor. This can be done to prevent
		// this item from receiving any tick events.
		void Detach();

	private:
		IUpdateProcessor* mAttachedToProcessor;
	};

	extern float32 SecondsPerTick;
}
