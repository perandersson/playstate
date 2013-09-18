#pragma once

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

#include "../types.h"
#include "script_collection.h"

namespace playstate
{
	class Scriptable;

	struct Vector3;
	struct Vector2;
	struct Point;
	struct Color;
	struct Rect;

	struct ResourceData;

	//
	// Pops a float value and returns the result from the lua stack. If the value at the top of the stack
	// is not a float value then return the defaultValue instead.
	//
	// @param L
	// @param defaultValue
	// @return
	extern float32 luaM_popfloat(lua_State* L, float32 defaultValue = 0);

	extern float32 luaM_tofloat(lua_State* L, int idx, float32 defaultValue = 0);

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
	// Pushes resource data onto the lua stack
	extern void luaM_pushresource(lua_State* L, ResourceData* resourceData);

	//
	// Retrieves the resource from the top of the stack
	extern ResourceData* luaM_getresourcedata(lua_State* L);

	extern ResourceData* luaM_popresourcedata(lua_State* L);

	//
	// Pops a script collection from the top of the stack
	extern ScriptCollection* luaM_popcollection(lua_State* L);

	//
	// Pops a Vector3 from the Lua stack.
	// A Vector3 can be represented as either a Lua table or as multiple parameters.
	// @return A three-dimensional vector. Vector3::Zero if the value in on the stack is invalid.
	extern Vector3 luaM_popvector3(lua_State* L);

	extern Vector2 luaM_popvector2(lua_State* L);

	//
	//
	extern uint32 luaM_pushvector3(lua_State* L, const Vector3& vec);
	
	//
	//
	extern uint32 luaM_pushvector2(lua_State* L, const Vector2& vec);

	//
	// @return The number of elements pushed to the stack
	extern uint32 luaM_pushpoint(lua_State* L, const Point& point);

	//
	extern Point luaM_poppoint(lua_State* L);

	extern Rect luaM_poprect(lua_State* L);

	//
	// @return The number of elements pushed to the stack
	extern uint32 luaM_pushcolor(lua_State* L, const Color& color);

	//
	// Pops the color from the top of the stack and returns the result. 
	// 
	extern Color luaM_popcolor(lua_State* L);

	//
	// Print an lua error like message without shutting down the script execution
	extern void luaM_printerror(lua_State* L, const char* msg, ...);
	
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

	template<class T>
	Resource<T> luaM_popresource(lua_State* L) {
		ResourceData* resource = luaM_popresourcedata(L);
		if(resource != NULL && Resource<T>::IsType(resource)) {
			return Resource<T>(resource);
		}
		
		return Resource<T>();
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
