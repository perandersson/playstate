#include "../../memory/memory.h"
#include "win32_thread_factory.h"
#include "win32_thread.h"
#include "win32_lock.h"
#include "win32_thread_event.h"
using namespace playstate;

template<> playstate::IThreadFactory* playstate::Singleton<playstate::IThreadFactory>::gSingleton = NULL;

Win32ThreadFactory::Win32ThreadFactory() : mCoreCount(0)
{
	// http://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	mCoreCount = sysinfo.dwNumberOfProcessors;
}

Win32ThreadFactory::~Win32ThreadFactory()
{
}

ILock* Win32ThreadFactory::CreateLock()
{
	return new Win32Lock();
}

IThreadEvent* Win32ThreadFactory::CreateThreadEvent(ILock* lock)
{
	return new Win32ThreadEvent(static_cast<Win32Lock*>(lock));
}

IThread* Win32ThreadFactory::CreateThread(IRunnable* runnable)
{
	return new Win32Thread(runnable);
}

uint32 Win32ThreadFactory::GetCoreCount() const
{
	return mCoreCount;
}
