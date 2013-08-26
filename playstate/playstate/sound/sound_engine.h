#pragma once

#include "../singleton.h"
#include "../types.h"
#include "../math/vector3.h"
#include "sound_effect.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class Music;

	class ISoundListener
	{
	public:
		virtual ~ISoundListener() {}

	public:
		virtual void SetPosition(const Vector3& position) = 0;
		virtual void LookAt(const Vector3& direction, const Vector3& up) = 0;
	};

	//
	//
	class ISoundEngine : public Singleton<ISoundEngine>
	{
	public:
		virtual void Play(SoundEffect* effect) = 0;
		virtual void Play(SoundEffect* effect, const Vector3& position) = 0;

		virtual void SetMasterVolume(float32 volume) = 0;
		virtual void SetMusicVolume(float32 volume) = 0;
		virtual void SetSoundEffectVolume(float32 volume) = 0;

		//
		// @return The one listening for sounds on the current device (usually the player).
		virtual ISoundListener* GetListener() = 0;
	};
	
	//
	// Script integration
	//

	extern int Sound_Play(lua_State* L);
	extern int Sound_PlayAtPos(lua_State* L);
	extern int Sound_SetMasterVolume(lua_State* L);
	extern int Sound_SetMusicVolume(lua_State* L);
	extern int Sound_SetSoundEffectVolume(lua_State* L);
	static luaL_Reg Sound_Methods[] = {
		{ "Play", Sound_Play },
		{ "PlayAtPos", Sound_PlayAtPos },
		{ "SetMasterVolume", Sound_SetMasterVolume },
		{ "SetMusicVolume", Sound_SetMusicVolume },
		{ "SetSoundEffectVolume", Sound_SetSoundEffectVolume },
		{ NULL, NULL }
	};

	extern int SoundListener_SetPosition(lua_State* L);
	extern int SoundListener_LookAt(lua_State* L);
	static luaL_Reg SoundListener_Methods[] = {
		{ "SetPosition", SoundListener_SetPosition },
		{ "LookAt", SoundListener_LookAt },
		{ NULL, NULL }
	};

}
