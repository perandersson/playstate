#pragma once

#include "../thread_factory.h"

namespace playstate
{
	class Win32ThreadFactory : public IThreadFactory
	{
	public:
		Win32ThreadFactory();
		virtual ~Win32ThreadFactory();

	// IThreadFactory
	public:
		virtual ILock* CreateLock();
		virtual IThreadEvent* CreateThreadEvent(ILock* lock);
		virtual IThread* CreateThread(IRunnable* runnable);
		virtual uint32 GetCoreCount() const;

	private:
		uint32 mCoreCount;
	};
}

