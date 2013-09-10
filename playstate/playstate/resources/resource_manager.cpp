#include "../memory/memory.h"
#include "resource_manager.h"
#include "../script/luam.h"
using namespace playstate;

template<> playstate::IResourceManager* playstate::Singleton<playstate::IResourceManager>::gSingleton = NULL;

int playstate::Resource_Load(lua_State* L)
{
	playstate::string path = lua_tostring(L, -1); lua_pop(L, 1);
	ResourceData* data = IResourceManager::Get().GetResourceData(path);
	luaM_pushresource(L, data);
	return 1;
}
