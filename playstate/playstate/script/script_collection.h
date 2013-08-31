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
		ScriptCollection();
		ScriptCollection(lua_State* L, script_ref scriptRef);
		ScriptCollection(const ScriptCollection& s);
		virtual ~ScriptCollection();
		
		//
		//
		ScriptCollection& operator = (const ScriptCollection& s);

	public:
		int32 FindInt(const playstate::character* key, int32 defaultVal = 0) const;
		float32 FindFloat(const playstate::character* key, float32 defaultVal = 0.0f) const;
		bool FindBool(const playstate::character* key, bool defaultVal = false) const;
		playstate::string FindString(const playstate::character* key, const playstate::character* defaultVal = "") const;

	protected:
		//
		// Find a key and pushes it to the top of the stack
		//
		// @return TRUE if the key is found; FALSE otherwise
		// @remark If no key is found then no element is pushed onto the stack
		bool FindKey(const playstate::character* key) const;

	protected:
		script_ref mScriptRef;
		lua_State* mLuaState;
	};
}
