#pragma once

#include <playstate/timer/timer_factory.h>

namespace playstate
{
	namespace win32
	{
		class Win32TimerFactory : public ITimerFactory
		{
		public:
			Win32TimerFactory();
			virtual ~Win32TimerFactory();

		// ITimerFactory
		public:
			virtual ITimer* CreateTimer();
		};
	}
}
