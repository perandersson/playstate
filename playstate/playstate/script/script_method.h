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
		// Invokes this method and returns the number of returned values
		uint32 Invoke();
		uint32 Invoke(uint32 p1);
		uint32 Invoke(uint32 p1, uint32 p2);

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
		bool PrepareMethod();

	private:
		script_ref mObjectID;
		script_ref mMethodID;
		lua_State* mCurrentState;
	};
}