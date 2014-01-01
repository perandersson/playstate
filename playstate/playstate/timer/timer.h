#pragma once

#include "../types.h"

namespace playstate
{
	class ITimer
	{
	public:
		virtual ~ITimer() {}

	public:
		//
		// Starts this timer
		virtual void Start() = 0;

		//
		// Returns the time it's been since the timer started and restart it afterwards.
		//
		// @return The time in seconds since the timer was started
		virtual float32 GetElapsedAndRestart() = 0;

		//
		// @return The total time in seconds between one start and one stop
		virtual float64 GetTotalTime() = 0;

		//
		// Stops this timer
		virtual void Stop() = 0;
	};
}
