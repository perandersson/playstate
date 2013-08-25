#pragma once

#include "../singleton.h"
#include "../types.h"
#include "../math/vector3.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class Music;
	class SoundEffect;

	class SoundFormat
	{
	public:
		enum Enum {
			UNKNOWN,
			MONO8,
			MONO16,
			STEREO8,
			STEREO16
		};

		static const Enum Default = SoundFormat::UNKNOWN;
	};

	class ISoundListener
	{
	public:
		virtual ~ISoundListener() {}

	public:
		virtual void SetPosition(const Vector3& position) = 0;
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
	static luaL_Reg Sound_Methods[] = {
		{ "Play", Sound_Play },
		{ NULL, NULL }
	};

	extern int SoundListener_SetPosition(lua_State* L);
	static luaL_Reg SoundListener_Methods[] = {
		{ "SetPosition", SoundListener_SetPosition },
		{ NULL, NULL }
	};

}
