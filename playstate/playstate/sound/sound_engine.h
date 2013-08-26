#pragma once

#include "../singleton.h"
#include "../types.h"
#include "../math/vector3.h"
#include "sound_effect.h"
#include "music.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
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
		//
		// Plays the supplied sound effect
		//
		// @param effect
		virtual void Play(SoundEffect* effect) = 0;

		//
		// Plays the supplied sound effect at the supplied position
		//
		// @param effect
		// @param position
		// @remark Playing positioned stereo sound effects are not possible. 
		//		The sound engine will print a warning message in the log and play this at the
		//		listeners position instead.
		virtual void Play(SoundEffect* effect, const Vector3& position) = 0;

		//
		// Starts playing a music resource
		//
		// @param music
		// @remark The game engine can play up to three music resources at the same time. Although not recommended for extended use,
		//	this feature greatly enhance smooth transitions between music tracks.
		virtual void Play(Music* music) = 0;

		//
		// Starts playing the supplied music resource
		//
		// @param fadeInTime
		virtual void Play(Music* music, float32 fadeInTime) = 0;

		virtual void Stop(Music* music) = 0;

		virtual void Stop(Music* music, float32 fadeOutTime) = 0;

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
