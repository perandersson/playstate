#pragma once

#include <playstate/thread/thread_event.h>
#include "win32_lock.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32ThreadEvent : public IThreadEvent
		{
		public:
			Win32ThreadEvent(Win32Lock* lock);
			virtual ~Win32ThreadEvent();

		// IThreadEvent
		public:
			virtual void Notify();
			virtual void Wait();
			virtual void Wait(uint64 timeout);
	
		private:
			CONDITION_VARIABLE mEventCondition;
			Win32Lock* mLock;
		};
	}
}

