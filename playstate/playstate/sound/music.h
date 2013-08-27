#pragma once

#include "../resources/resource_object.h"
#include "../linked_list.h"
#include "../types.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class ISoundEngine;

	//
	// Class which represents a music resource in the game engine. 
	class Music : public ResourceObject
	{
	public:
		Music(float32 duration);
		virtual ~Music();

		//
		// @return The duration of this music resource
		inline float32 GetDuration() const {
			return mDuration;
		}

	private:
		float32 mDuration;
	};

	
	//
	// Script integration
	//

	extern int Music_Load(lua_State* L);
	extern int Music_GetDuration(lua_State* L);
	static luaL_Reg Music_Methods[] = {
		{ "Load", Music_Load },
		{ "GetDuration", Music_GetDuration },
		{ NULL, NULL }
	};
}
