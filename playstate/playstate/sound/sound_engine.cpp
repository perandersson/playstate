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

int playstate::Sound_PlayAtPos(lua_State* L)
{
	if(lua_gettop(L) < 2) {
		luaM_printerror(L, "Expected: Sound.PlayAtPos(ResourceData, Vector3)");
		return 0;
	}
	Vector3 pos = luaM_popvector3(L);
	ResourceData* resourceData = luaM_popresource(L);
	if(resourceData != NULL) {
		Resource<SoundEffect> effect(resourceData);
		ISoundEngine::Get().Play(effect.Get(), pos);
	} else {
		luaM_printerror(L, "Expected: Sound.PlayAtPos(ResourceData, Vector3)");
	}
	return 0;
}

int playstate::Sound_SetMasterVolume(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Sound.SetMasterVolume(number)");
		return 0;
	}

	float32 volume = lua_tonumber(L, -1); lua_pop(L, 1);
	ISoundEngine::Get().SetMasterVolume(volume);
	return 0;
}

int playstate::Sound_SetMusicVolume(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Sound.SetMusicVolume(number)");
		return 0;
	}

	float32 volume = lua_tonumber(L, -1); lua_pop(L, 1);
	ISoundEngine::Get().SetMusicVolume(volume);
	return 0;
}

int playstate::Sound_SetSoundEffectVolume(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: Sound.SetSoundEffectVolume(number)");
		return 0;
	}

	float32 volume = lua_tonumber(L, -1); lua_pop(L, 1);
	ISoundEngine::Get().SetSoundEffectVolume(volume);
	return 0;
}

int playstate::SoundListener_SetPosition(lua_State* L)
{
	Vector3 pos = luaM_popvector3(L);
	ISoundEngine::Get().GetListener()->SetPosition(pos);
	return 0;
}

int playstate::SoundListener_LookAt(lua_State* L)
{
	Vector3 up = luaM_popvector3(L);
	Vector3 direction = luaM_popvector3(L);
	ISoundEngine::Get().GetListener()->LookAt(direction, up);
	return 0;
}
