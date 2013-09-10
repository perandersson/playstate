#include "../memory/memory.h"
#include "music.h"
#include "../resources/resource_manager.h"
#include "../script/luam.h"
using namespace playstate;

Music::Music(float32 duration)
	: mDuration(duration)
{
}

Music::~Music()
{
}

int playstate::Music_GetDuration(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Music.GetDuration(music)");
		lua_pushnumber(L, 0.0);
		return 1;
	}
	
	Resource<Music> music = luaM_popresource<Music>(L);
	if(!music.IsNull()) {
		lua_pushnumber(L, music->GetDuration());
	} else {
		luaM_printerror(L, "Expected: Music.GetDuration(music)");
		lua_pushnumber(L, 0.0);
	}
	return 1;
}
