#pragma once
#include "../singleton.h"
#include "wrapper/thread_wrapper.h"
#include "wrapper/lock_wrapper.h"
#include "thread_event.h"
#include "runnable.h"

namespace playstate
{
	//
	// 
	class IThreadFactory : public Singleton<IThreadFactory>
	{
	public:
		//
		// Creates a new unlocked lock.
		virtual ILock* CreateLock() = 0;

		//
		// Creates a new event.
		// @param lock Attaches a lock to the event. Useful to prevent deadlocks
		//		together with events.
		virtual IThreadEvent* CreateThreadEvent(ILock* lock) = 0;

		//
		// Createa a new non-started thread.
		virtual IThread* CreateThread(IRunnable* runnable) = 0;

		//
		// Retrieves the number of cores available on the current computer
		virtual uint32 GetCoreCount() const = 0;
	};
}
