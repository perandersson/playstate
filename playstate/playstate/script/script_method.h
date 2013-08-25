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
		ScriptMethod(uint32 id, uint32 method, lua_State* L);
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
		uint32 mObjectID;
		uint32 mMethodID;
		lua_State* mCurrentState;
		uint32 mNumResults;
	};
}