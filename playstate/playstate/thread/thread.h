#pragma once

#include "../types.h"

namespace playstate
{
	enum ThreadPriority
	{
		PRIORITY_LOW = 0,
		PRIORITY_NORMAL,
		PRIORITY_HIGH
	};

	enum ThreadState
	{
		THREAD_RUNNING,
		THREAD_SUSPENDED,
		THREAD_STOPPING,
		THREAD_STOPPED,
	};

	class IThread
	{
	public:
		virtual ~IThread() {}

		//
		// @return The state of this thread
		virtual ThreadState GetState() const = 0;
		
		//
		// Starts this thread. It does nothing if it's already started
		// @remark This method will notify this thread to wake up if it's running.
		virtual void Start() = 0;

		//
		// Stops this thread and waits (indefinitely) until it's stopped running. It does nothing if it's already stopped/stopping
		// @remark This method will notify this thread to wake up if it's running.
		virtual void StopAndWait() = 0;
		
		//
		// Stops this thread and waits until it's stopped running. It does nothing if it's already stopped/stopping.
		// If the timeout (in milliseconds) is reached then the execution thread continues.
		// @remark This method will notify this thread to wake up if it's running.
		virtual void StopAndWait(uint64 timeoutInMillis) = 0;
		
		//
		// Set the priority of the thread
		virtual void SetPriority(ThreadPriority priority) = 0;

		//
		// Notifies this thread to continue (if the wait method has been invoked).
		// @remark If this method is called before the wait method then that wait method won't suspend the thread.
		virtual void Notify() = 0;
		
		//
		// Wait until this thread is notified (i.e. the thread will be suspended until the Notify method is invoked)
		// @remark This call will make the thread wait indefinitely
		virtual void Wait() = 0;

		//
		// Wait until this thread is notified (i.e. the thread will be suspended until the Notify method is invoked)
		// @param timeout How long we should be allowed to wait until continuing anyways
		virtual void Wait(uint64 timeoutInMillis) = 0; 
	};
}
