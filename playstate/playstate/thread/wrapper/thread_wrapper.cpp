#include "../../memory/memory.h"
#include "../thread_factory.h"
#include "../../rendering/graphics_driver.h"
using namespace playstate;

Thread::Thread(IRunnable* runnable)
{
	mThread = IThreadFactory::Get().CreateThread(runnable);
}

Thread::~Thread()
{
	delete mThread;
	mThread = NULL;
}

ThreadState Thread::GetState() const
{
	return mThread->GetState();
}

void Thread::Start()
{
	mThread->Start();
}

void Thread::StopAndWait()
{
	mThread->StopAndWait();
}

void Thread::StopAndWait(uint64 timeout)
{
	mThread->StopAndWait(timeout);
}

void Thread::SetPriority(ThreadPriority priority)
{
	mThread->SetPriority(priority);
}

void Thread::Notify()
{
	mThread->Notify();
}

void Thread::Wait()
{
	mThread->Wait();
}

void Thread::Wait(uint64 timeout)
{
	mThread->Wait(timeout);
}

/////////////////////////////


class RenderAwareRunnableWrapper : public IRunnable
{
public:
	RenderAwareRunnableWrapper(IRunnable* runnable)
		: mRunnable(runnable)
	{
		mRenderContext = IGraphicsDriver::Get().CreateRenderContext();
	}

	virtual ~RenderAwareRunnableWrapper()
	{
		delete mRenderContext;
		mRenderContext = NULL;
	}

// IRunnable
public:
	virtual void Run(IThread& thread)
	{
		mRenderContext->MakeCurrent();
		mRunnable->Run(thread);
		mRenderContext->Unbind();
	}

private:
	IRunnable* mRunnable;
	IRenderContext* mRenderContext;
};

RenderAwareThread::RenderAwareThread(IRunnable* runnable)
	: mRunnable(new RenderAwareRunnableWrapper(runnable))
{
	mThread = IThreadFactory::Get().CreateThread(mRunnable);
}

RenderAwareThread::~RenderAwareThread()
{
	delete mThread;
	mThread = NULL;

	if(mRunnable != NULL) {
		delete mRunnable;
		mRunnable = NULL;
	}
}

ThreadState RenderAwareThread::GetState() const
{
	return mThread->GetState();
}

void RenderAwareThread::Start()
{
	mThread->Start();
}

void RenderAwareThread::StopAndWait()
{
	mThread->StopAndWait();
}

void RenderAwareThread::StopAndWait(uint64 timeout)
{
	mThread->StopAndWait(timeout);
}

void RenderAwareThread::SetPriority(ThreadPriority priority)
{
	mThread->SetPriority(priority);
}

void RenderAwareThread::Notify()
{
	mThread->Notify();
}

void RenderAwareThread::Wait()
{
	mThread->Wait();
}

void RenderAwareThread::Wait(uint64 timeout)
{
	mThread->Wait(timeout);
}
