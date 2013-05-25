#pragma once

#include "../thread.h"
#include "../runnable.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	class Win32Thread : public IThread
	{
	public:
		Win32Thread(IRunnable* runnable);
		virtual ~Win32Thread();

	// IThread
	public:
		virtual ThreadState GetState() const;
		virtual void Start();
		virtual void StopAndWait();
		virtual void StopAndWait(uint64 timeout);
		virtual void SetPriority(ThreadPriority priority);
		virtual void Notify();
		virtual void Wait();
		virtual void Wait(uint64 timeout); 

	private:
		HANDLE mThreadHandle;
		IRunnable* mRunnable;
		ThreadPriority mCurrentPriority;
		ThreadState mThreadState;

		CONDITION_VARIABLE mEventCondition;
		CRITICAL_SECTION mCriticalSection;
	};
}
