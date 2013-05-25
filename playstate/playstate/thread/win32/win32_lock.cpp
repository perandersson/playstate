#include "../../memory/memory.h"
#include "win32_lock.h"
using namespace playstate;

Win32Lock::Win32Lock()// : mLockHandle(NULL)
{
	//mLockHandle = CreateMutex(NULL, FALSE, NULL);
	InitializeCriticalSection(&mCriticalSection);
}

Win32Lock::~Win32Lock()
{
}

CRITICAL_SECTION* Win32Lock::GetCriticalSection()
{
	return &mCriticalSection;
}

void Win32Lock::Lock()
{
	EnterCriticalSection(&mCriticalSection);
}

void Win32Lock::Unlock()
{
	LeaveCriticalSection(&mCriticalSection);
}
