#include "../../memory/memory.h"
#include "win32_thread_event.h"
using namespace playstate;

Win32ThreadEvent::Win32ThreadEvent(Win32Lock* lock) : mLock(lock)
{
	InitializeConditionVariable(&mEventCondition);
}

Win32ThreadEvent::~Win32ThreadEvent()
{
}

void Win32ThreadEvent::Notify()
{
	WakeConditionVariable(&mEventCondition);
}

void Win32ThreadEvent::Wait()
{
	Wait(INFINITE);
}

void Win32ThreadEvent::Wait(uint64 timeout)
{
	SleepConditionVariableCS(&mEventCondition, mLock->GetCriticalSection(), (DWORD)timeout);
}
