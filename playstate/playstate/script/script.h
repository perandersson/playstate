#pragma once

#include "exception/script_exception.h"
#include "luam.h"
#include "script_collection.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class Script
	{
	public:
		Script(lua_State* L, uint32 numResults);
		virtual ~Script();

	public:
		//
		// Evaluate an expression on the the loaded file. 
		void Evaluate(const char* fml, ...);

		//
		// Executes an expression on the loaded script file.
		void Execute(const char* expression);
		
		// 
		// 
		template<class T>
		T* ReadInstance() {
			if(mNumResults == 0)
				return NULL;

			mNumResults--;
			return luaM_popobject<T>(mLuaState);
		}

		//
		//
		ScriptCollection ReadCollection();


	private:
		lua_State* mLuaState;
		uint32 mNumResults;
	};
}
