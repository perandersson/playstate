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

int playstate::Music_Load(lua_State* L)
{
	playstate::string path = lua_tostring(L, -1); lua_pop(L, 1);
	Resource<Music> music = IResourceManager::Get().GetResource<Music>(path);
	luaM_pushresource(L, music.GetResourceData());
	return 1;
}

int playstate::Music_GetDuration(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Music.GetDuration(music)");
		lua_pushnumber(L, 0.0);
		return 1;
	}
	
	ResourceData* resourceData = luaM_popresource(L);
	if(resourceData != NULL) {
		Resource<Music> music(resourceData);
		lua_pushnumber(L, music->GetDuration());
	} else {
		luaM_printerror(L, "Expected: Music.GetDuration(music)");
		lua_pushnumber(L, 0.0);
	}
	return 1;
}
