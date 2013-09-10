#pragma once

#include "../resources/resource_object.h"
#include "../types.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
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

	//
	// This class symbolizes a sound effect inside the game engine. 
	// Use the class {@code playstate::ISoundEngine} to play this sound effect.
	class SoundEffect : public ResourceObject
	{
	public:
		SoundEffect(SoundFormat::Enum format, float32 duration);
		virtual ~SoundEffect();

		//
		// @return This sound effects duration in seconds.
		inline float32 GetDuration() const {
			return mDuration;
		}

		//
		// @return This sound effects format
		inline SoundFormat::Enum GetFormat() const {
			return mFormat;
		}
	
	private:
		SoundFormat::Enum mFormat;
		float32 mDuration;
	};
	
	//
	// Script integration
	//

	extern int SoundEffect_Load(lua_State* L);
	extern int SoundEffect_GetDuration(lua_State* L);
	static luaL_Reg SoundEffect_Methods[] = {
		{ "GetDuration", SoundEffect_GetDuration },
		{ NULL, NULL }
	};
}
