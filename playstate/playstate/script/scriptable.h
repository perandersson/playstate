#pragma once

#include "../types.h"
#include "luam.h"
#include "script_method.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class Scriptable
	{
	protected:
		//
		// 
		Scriptable();
		
		//
		// @return A script method; NULL if no method was found with the supplied name
		ScriptMethod* GetMethod(const char* name);

	protected:
		//
		virtual void OnRegistered();

	public:
		virtual ~Scriptable();

		// 
		// Attaches this object to the supplied lua state.
		//
		// @param L The current lua state
		// @param id The unique script reference for this object.
		bool RegisterObject(lua_State* L, script_ref id);

		//
		// Check if this instance is bound to the script engine.
		//
		// @return TRUE if this object is bound to the script engine; FALSE otherwise.
		bool IsScriptBound() const;

		// 
		// @return This object ID
		const script_ref GetID() const;
		
	protected:
		script_ref mScriptRef;
		lua_State* mCurrentState;
	};
}
