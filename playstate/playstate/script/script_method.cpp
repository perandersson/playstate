#include "../memory/memory.h"
#include "script_method.h"
#include "luam.h"
using namespace playstate;

ScriptMethod::ScriptMethod(script_ref id, script_ref method, lua_State* L)
	: mObjectID(id), mMethodID(method), mCurrentState(L)
{
	assert(method != 0 && "Invalid method ID");
	assert(id != 0 && "Invalid object reference");
}

ScriptMethod::~ScriptMethod()
{
	luaL_unref(mCurrentState, LUA_REGISTRYINDEX, mMethodID);
}

uint32 ScriptMethod::Invoke()
{
	lua_State* L = mCurrentState;

	int beforeCall = lua_gettop(L);
	if(PrepareMethod()) {
		if(lua_pcall(L, 1, LUA_MULTRET, NULL) == 0) {
			return lua_gettop(L) - beforeCall;
		} else {
			const char* err = lua_tostring(L, -1);
			luaM_printerror(L, err);
			lua_pop(L, 1);
			return 0;
		}
	}
	return 0;
}

uint32 ScriptMethod::Invoke(uint32 p1)
{
	lua_State* L = mCurrentState;

	int beforeCall = lua_gettop(L);
	if(PrepareMethod()) {
		lua_pushinteger(L, p1);
		if(lua_pcall(L, 2, LUA_MULTRET, NULL) == 0) {
			return lua_gettop(L) - beforeCall;
		} else {
			const char* err = lua_tostring(L, -1);
			luaM_printerror(L, err);
			lua_pop(L, 1);
			return 0;
		}
	}
	return 0;
}

uint32 ScriptMethod::Invoke(uint32 p1, uint32 p2)
{
	lua_State* L = mCurrentState;

	int beforeCall = lua_gettop(L);
	if(PrepareMethod()) {
		lua_pushinteger(L, p1);
		lua_pushinteger(L, p2);
		if(lua_pcall(L, 3, LUA_MULTRET, NULL) == 0) {
			return lua_gettop(L) - beforeCall;
		} else {
			const char* err = lua_tostring(L, -1);
			luaM_printerror(L, err);
			lua_pop(L, 1);
			return 0;
		}
	}
	return 0;
}

bool ScriptMethod::PrepareMethod()
{
	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mMethodID);
	if(lua_isfunction(mCurrentState, -1)) {
		lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mObjectID);
		return true;
	}
	lua_pop(mCurrentState, 1);
	return false;
}

int32 ScriptMethod::GetInt32()
{
	int32 result = lua_tointeger(mCurrentState, -1); lua_pop(mCurrentState, 1);
	return result;
}

uint32 ScriptMethod::GetUInt32()
{
	uint32 result = lua_tointeger(mCurrentState, -1); lua_pop(mCurrentState, 1);
	return result;
}

float32 ScriptMethod::GetFloat32()
{
	float32 result = (float32)lua_tonumber(mCurrentState, -1); lua_pop(mCurrentState, 1);
	return result;
}

float64 ScriptMethod::GetFloat64()
{
	float64 result = lua_tonumber(mCurrentState, -1); lua_pop(mCurrentState, 1);
	return result;
}

playstate::string ScriptMethod::GetString()
{
	playstate::string result = lua_tostring(mCurrentState, -1); lua_pop(mCurrentState, 1);
	return result;
}

Scriptable* ScriptMethod::GetObject()
{
	Scriptable* result = luaM_getinstance(mCurrentState); lua_pop(mCurrentState, 1);
	return result;
}

bool ScriptMethod::GetBool()
{
	bool result = lua_toboolean(mCurrentState, -1) == 1; lua_pop(mCurrentState, 1);
	return result;
}
