#include "../memory/memory.h"
#include "sound_engine.h"
#include "sound_effect.h"
#include "../resources/resource_manager.h"
using namespace playstate;

template<> playstate::ISoundEngine* playstate::Singleton<playstate::ISoundEngine>::gSingleton = NULL;

int playstate::Sound_Play(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Sound.Play(ResourceData)");
		return 0;
	}
		
	ResourceData* resourceData = luaM_popresource(L);
	if(resourceData != NULL) {
		Resource<SoundEffect> effect(resourceData);
		ISoundEngine::Get().Play(effect.Get());
	} else {
		luaM_printerror(L, "Expected: Sound.Play(ResourceData)");
	}
	return 0;
}

int playstate::SoundListener_SetPosition(lua_State* L)
{
	Vector3 pos = luaM_popvector3(L);
	ISoundEngine::Get().GetListener()->SetPosition(pos);
	return 0;
}
