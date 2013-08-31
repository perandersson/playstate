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
	class Scriptable;

	class ScriptMethod
	{
	public:
		ScriptMethod(script_ref id, script_ref method, lua_State* L);
		~ScriptMethod();

		//
		// Invokes this method
		bool Invoke();

		//
		// @return An integer at the supplied index.
		int32 GetInt32();
		uint32 GetUInt32();

		float32 GetFloat32();
		float64 GetFloat64();

		playstate::string GetString();

		Scriptable* GetObject();

		bool GetBool();

	private:
		script_ref mObjectID;
		script_ref mMethodID;
		lua_State* mCurrentState;
		uint32 mNumResults;
	};
}