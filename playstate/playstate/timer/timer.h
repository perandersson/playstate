#pragma once

#include "../types.h"

namespace playstate
{
	class ITimer
	{
	public:
		virtual ~ITimer() {}

	public:
		virtual void Start() = 0;

		virtual float32 GetElapsedAndRestart() = 0;

		virtual float64 GetTotalTime() = 0;

		virtual void Stop() = 0;
	};
}
