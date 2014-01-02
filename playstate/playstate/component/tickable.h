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
		LinkedListLink<Tickable> TickableLink;

	public:
		Tickable();
		virtual ~Tickable();
	};

	extern float32 SecondsPerTick;
}
