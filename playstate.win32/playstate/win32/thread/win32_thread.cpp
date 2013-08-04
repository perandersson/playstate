#include <playstate/memory/memory.h>
#include "win32_thread.h"

#include <cassert>

using namespace playstate;
using namespace playstate::win32;

struct Win32ThreadParam
{
	IRunnable* Runnable;
	IThread* Thread;
};

DWORD WINAPI _ThreadToRunnable(LPVOID param)
{
	Win32ThreadParam* threadParam = reinterpret_cast<Win32ThreadParam*>(param);
	threadParam->Runnable->Run(*threadParam->Thread);
	delete threadParam;
	return 0;
}

Win32Thread::Win32Thread(IRunnable* runnable) : mThreadHandle(NULL), mRunnable(runnable), 
	mThreadState(THREAD_STOPPED), mCurrentPriority(PRIORITY_NORMAL)
{
	assert_not_null(runnable);

	InitializeConditionVariable(&mEventCondition);
	InitializeCriticalSection(&mCriticalSection);
}

Win32Thread::~Win32Thread()
{
	StopAndWait();

	if(mThreadHandle != NULL) {
		CloseHandle(mThreadHandle);
		mThreadHandle = NULL;
	}
}

HANDLE Win32Thread::GetHandle() const
{
	return mThreadHandle;
}

ThreadState Win32Thread::GetState() const
{
	return mThreadState;
}

void Win32Thread::Start()
{
	if(mThreadState != THREAD_STOPPED) {
		Notify();
		return;
	}

	mThreadState = THREAD_RUNNING;
	Win32ThreadParam* param = new Win32ThreadParam;
	param->Runnable = mRunnable;
	param->Thread = this;
	mThreadHandle = CreateThread(NULL, 0, _ThreadToRunnable, param, 0, NULL);
	SetThreadPriority(mThreadHandle, THREAD_PRIORITY_NORMAL);
}

void Win32Thread::StopAndWait()
{
	StopAndWait(INFINITE);
}

void Win32Thread::StopAndWait(uint64 timeout)
{ 
	if(mThreadState != THREAD_RUNNING)
		return;

	mThreadState = THREAD_STOPPING;
	Notify();
	WaitForMultipleObjects(1, &mThreadHandle, true, (DWORD)timeout);
	mThreadState = THREAD_STOPPED;
}

void Win32Thread::Notify()
{
	WakeConditionVariable(&mEventCondition);
}

void Win32Thread::Wait()
{
	Wait(INFINITE);
}

void Win32Thread::Wait(uint64 timeout)
{
	SleepConditionVariableCS(&mEventCondition, &mCriticalSection, (DWORD)timeout);
}

void Win32Thread::SetPriority(ThreadPriority priority)
{
	const int p[] = {
		THREAD_PRIORITY_LOWEST,
		THREAD_PRIORITY_NORMAL,
		THREAD_PRIORITY_HIGHEST
	};

	if(priority != mCurrentPriority) {
		SetThreadPriority(mThreadHandle, p[(int)priority]);
		mCurrentPriority = priority;
	}
}
