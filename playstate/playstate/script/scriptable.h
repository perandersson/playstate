#pragma once

#include "../types.h"
#include "luam.h"
#include <vector>

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

		typedef std::vector<uint32> MethodIds;

	private:
		

	protected:
		//
		// 
		Scriptable();
		
		//
		// Finds and pushes the supplied method and the "self" parameter to the lua stack.
		bool PrepareMethod(const char* method);

		//
		// Checks if this instance has the method with the supplied name
		// 
		// @param method The method
		// @return 
		bool HasMethod(const char* method);

		//
		// Finds and pushes the supplied method and the "self" parameter to the lua stack.
		bool PrepareMethod(uint32 methodID);

		//
		// @return ID number for the method
		uint32 GetMethodID(const char* method);

	protected:
		//
		virtual void OnRegistered();

	public:
		virtual ~Scriptable();

		// 
		// Attaches this object to the supplied lua state
		bool RegisterObject(lua_State* L, uint32 id);

		//
		// Check if this instance is bound to the script engine.
		// @return TRUE if this object is bound to the script engine; FALSE otherwise.
		bool IsScriptBound() const;

		// 
		// @return This object ID
		const uint32 GetID() const;
		
	protected:
		uint32 mScriptRef;
		lua_State* mCurrentState;

		MethodIds mMethodIds;
	};
}
