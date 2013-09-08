#include "../memory/memory.h"
#include "timer_factory.h"

template<> playstate::ITimerFactory* playstate::Singleton<playstate::ITimerFactory>::gSingleton = NULL;

int playstate::ITimer_Start(lua_State* L)
{
	ITimer* timer = ITimerFactory::Get().CreateTimer();
	ITimer** ptr = (ITimer**)lua_newuserdata(L, sizeof(ITimer**));
	*ptr = timer;
	timer->Start();
	return 1;
}

int playstate::ITimer_End(lua_State* L)
{
	if(!lua_isuserdata(L, -1)) {
		luaM_printerror(L, "Expected: Timer.End(timer)");
		lua_pushnumber(L, 0);
		return 1;
	}

	ITimer** ptr = (ITimer**)lua_touserdata(L, -1);
	(*ptr)->Stop();
	float64 totalTime = (*ptr)->GetTotalTime();
	delete *ptr;
	lua_pushnumber(L, totalTime);
	return 1;
}
