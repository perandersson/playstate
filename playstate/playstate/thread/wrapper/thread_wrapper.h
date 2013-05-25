#pragma once

#include "../thread.h"

namespace playstate
{
	class IRunnable;

	//
	// Creates a suspended thread.
	class Thread : public IThread
	{
	public:
		//
		// @param runnable The runnable instance we want to execute in the other thread
		Thread(IRunnable* runnable);

		//
		// Destructor
		virtual ~Thread();

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
		IThread* mThread;
	};
	
	//
	// Creates a thread that's aware of the rendering system. Use this thread
	// if you want to support rendering in the new thread.
	class RenderAwareThread : public IThread
	{
	public:
		//
		// @param runnable The runnable instance we want to execute in the other thread
		RenderAwareThread(IRunnable* runnable);

		//
		// Destructor
		virtual ~RenderAwareThread();

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
		IRunnable* mRunnable;
		IThread* mThread;
	};
}
