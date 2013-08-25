#pragma once

#include "../resources/resource_object.h"
#include "../script/scriptable.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace playstate
{
	//
	// 
	class SoundEffect : public ResourceObject, public Scriptable
	{
	public:
		SoundEffect(ALuint bufferId, float32 duration);
		~SoundEffect();

		//
		//
		inline ALuint GetBufferID() const {
			return mBufferID;
		}

		inline float32 GetDuration() const {
			return mDuration;
		}
	
	protected:

	private:
		ALuint mBufferID;
		float32 mDuration;
	};
	
	//
	// Script integration
	//

	extern int SoundEffect_Load(lua_State* L);
	extern int SoundEffect_GetDuration(lua_State* L);
	static luaL_Reg SoundEffect_Methods[] = {
		{ "Load", SoundEffect_Load },
		{ "GetDuration", SoundEffect_GetDuration },
		{ NULL, NULL }
	};
}
