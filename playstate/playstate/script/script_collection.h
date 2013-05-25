#pragma once

#include "../types.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class ScriptCollection
	{
	public:
		ScriptCollection(lua_State* L, uint32 scriptRef);
		ScriptCollection(const ScriptCollection& s);
		~ScriptCollection();

	public:
		int32 FindInt(const char* key, int32 defaultVal = 0) const;
		float32 FindFloat(const char* key, float32 defaultVal = 0.0f) const;
		bool FindBool(const char* key, bool defaultVal = false) const;
		std::string FindString(const char* key, const char* defaultVal = "") const;

	private:
		//
		// Find a key
		bool FindKey(const char* key) const;

	private:
		uint32 mScriptRef;
		lua_State* mLuaState;
	};
}
