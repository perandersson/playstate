#pragma once

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class Scriptable;

	//
	// Sets/Replaces the supplied data value as the native instance for the table at the top of the stack
	// @param L
	// @param data
	extern void luaM_setinstance(lua_State* L, Scriptable* data);

	//
	// Retrieves the native instance value from the table at the top of the stack
	// @remark This does NOT pop the element at the top of the stack. 
	// @param L
	// @return A pointer to the data where the native instance is located in memory. Returns NULL if 
	//	the value at the top of the stack is not a table or if the instance reference is not set.
	extern Scriptable* luaM_getinstance(lua_State *L);

	//
	// Creates a script representation of a class and pushes it on the top of the stack.
	extern void luaM_createclass(lua_State* L, const char* className);

	//
	// Pushes the supplied object to the top of the stack. If the object isn't bound/registered to LUA
	// then it will create a script representation of the class for you.
	extern void luaM_pushobject(lua_State* L, const char* className, Scriptable* object);

	//
	// Pops an object parameter from the stack and returns the result
	template<class T>
	T* luaM_popobject(lua_State* L) {
		Scriptable* ptr = luaM_getinstance(L); lua_pop(L, 1);
		if(ptr != NULL) {
			return dynamic_cast<T*>(ptr);
		}
		return NULL;
	}

	//
	// Retrieves the object but doesn't pop it ffrom the stack.
	template<class T>
	T* luaM_getobject(lua_State* L) {
		Scriptable* ptr = luaM_getinstance(L);
		if(ptr != NULL) {
			return (T*)ptr;
		}
		return NULL;
	}
}


#ifndef LUA_CONSTRUCTOR
#define LUA_CONSTRUCTOR "__call"
#endif

#ifndef LUA_INHERIT_CONSTRUCTOR
#define LUA_INHERIT_CONSTRUCTOR "__init"
#endif

#ifndef LUA_TOSTRING
#define LUA_TOSTRING "__tostring"
#endif

#ifndef LUA_GC
#define LUA_GC "__gc"
#endif
