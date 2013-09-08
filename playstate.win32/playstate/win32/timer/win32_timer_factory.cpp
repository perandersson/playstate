#include <playstate/memory/memory.h>
#include "win32_timer_factory.h"
#include "win32_timer.h"
using namespace playstate;
using namespace playstate::win32;

Win32TimerFactory::Win32TimerFactory()
{
}

Win32TimerFactory::~Win32TimerFactory()
{
}

ITimer* Win32TimerFactory::CreateTimer()
{
	return new Win32Timer();
}
