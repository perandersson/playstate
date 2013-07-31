#pragma once

#include <playstate/thread/lock.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32Lock : public ILock
		{
		public:
			Win32Lock();
			~Win32Lock();

			CRITICAL_SECTION* GetCriticalSection();

		// ILock
		public:
			virtual void Lock();
			virtual void Unlock();

		private:
			CRITICAL_SECTION mCriticalSection;
		};
	}
}

