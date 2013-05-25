#pragma once

#include "../types.h"

namespace playstate
{
	class IThreadEvent
	{
	public:
		virtual ~IThreadEvent() {}

		virtual void Notify() = 0;

		//
		// Wait indefinitely until this event is notified.
		virtual void Wait() = 0;
		
		//
		// Wait until this event is notified or if the timeout has been reached.
		// @param timeoutInMillis
		virtual void Wait(uint64 timeoutInMillis) = 0; 
	};
}
