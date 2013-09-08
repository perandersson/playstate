#pragma once

#include "../singleton.h"
#include "timer.h"

namespace playstate
{
	class ITimerFactory : public Singleton<ITimerFactory>
	{
	public:
		virtual ITimer* CreateTimer() = 0;
	};
}
