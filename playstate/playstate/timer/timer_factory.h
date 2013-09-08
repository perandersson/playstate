#pragma once

#include "../singleton.h"
#include "timer.h"

#include "../script/scriptable.h"

namespace playstate
{
	class ITimerFactory : public Singleton<ITimerFactory>
	{
	public:
		virtual ITimer* CreateTimer() = 0;
	};

	
	//
	// Scripts
	//
	extern int ITimer_Start(lua_State* L);
	extern int ITimer_End(lua_State* L);
	static luaL_Reg ITimer_Methods[] = {
		{ "Start", ITimer_Start },
		{ "End", ITimer_End },
		{ NULL, NULL }
	};
}
