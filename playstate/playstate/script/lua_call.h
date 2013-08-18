#pragma once

#include "../types.h"
#include "exception/script_exception.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	struct Void {};

	template<typename T>
	struct to_type
	{
		typedef typename T type;
	};

	template<>
	struct to_type<void>
	{
		typedef Void type;
	};

class LuaCallBase
{

public:
	LuaCallBase(lua_State *luaState, uint32 methodID)
		: L(luaState), mMethodID(methodID)
	{}

protected:
	void push(const int32 &value)
	{
		lua_pushinteger(L, value);
	}

	void push(const float32& value)
	{
		lua_pushnumber(L, value);
	}

	void push(const float64& value)
	{
		lua_pushnumber(L, value);
	}

	void push(const playstate::string& value)
	{
		lua_pushstring(L, value.c_str());
	}

	void get(int32& value) const
	{
		value = lua_tointeger(L, -1);
	}

	void get(float32& value) const
	{
		value = lua_tonumber(L, -1);
	}

	void get(float64& value) const
	{
		value = lua_tonumber(L, -1);
	}

	void get(playstate::string& value) const 
	{
		 value = (char*)lua_tostring(L, -1);
	}

	void get(Void& value) const 
	{
	}

protected:
	lua_State *L;
	uint32 mMethodID;
};

template<typename TR, typename T1 = Void, typename T2 = Void, typename T3 = Void, typename T4 = Void>
class LuaCall : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	TR call(T1 a1, T2 a2, T3 a3, T4 a4)
	{
		TR returnValue;

		push(a1);
		push(a2);
		push(a3);
		push(a4);

		if(lua_pcall(L, 4, 1, 0) != 0)
		{
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}

		get(returnValue);
		return returnValue;
	}
};


template<typename TR, typename T1, typename T2, typename T3>
class LuaCall<TR,T1,T2,T3,Void> : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	TR call(T1 a1, T2 a2, T3 a3)
	{
		TR returnValue;

		push(a1);
		push(a2);
		push(a3);

		if(lua_pcall(L, 3, 1, 0) != 0) {
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}

		get(returnValue);
		return returnValue;
	}
};

// specialization to end deriving recursion
template<typename TR, typename T1, typename T2>
class LuaCall<TR,T1,T2,Void,Void> : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	TR call(T1 a1, T2 a2)
	{
		TR returnValue;

		push(a1);
		push(a2);

		if(lua_pcall(L, 2, 1, 0) != 0) {
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}

		get(returnValue);
		return returnValue;
	}
};

// specialization to end deriving recursion
template <typename TR, typename T1>
class LuaCall<TR,T1,Void,Void,Void> : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	TR call(T1 a1)
	{
		TR returnValue;

		push(a1);

		if(lua_pcall(L, 1, 1, 0) != 0) {
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}

		get(returnValue);
		return returnValue;
	}
};

template<typename TR>
class LuaCall<TR,Void,Void,Void,Void> : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	TR call(void)
	{
		TR returnValue;

		if(lua_pcall(L, 0, 1, 0) != 0) {
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}

		get(returnValue);
		return returnValue;
	}
};

template <>
class LuaCall<Void,Void,Void,Void,Void> : public LuaCallBase
{
public:
	LuaCall(lua_State *L, uint32 methodID) : LuaCallBase(L, methodID) {}

	void call(void)
	{
		if(lua_pcall(L, 0, 0, 0) != 0) {
			playstate::string err = lua_tostring(L, -1); lua_pop(L, 1);
			THROW_EXCEPTION(ScriptException, "Could not invoke method. Reason: %s", err.c_str());
		}
	}
};
}