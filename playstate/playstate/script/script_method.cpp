#include "../memory/memory.h"
#include "script_method.h"
#include "luam.h"
using namespace playstate;

ScriptMethod::ScriptMethod(script_ref id, script_ref method, lua_State* L)
	: mObjectID(id), mMethodID(method), mCurrentState(L), mNumResults(0)
{
	assert(method != 0 && "Invalid method ID");
	assert(id != 0 && "Invalid object reference");
}

ScriptMethod::~ScriptMethod()
{
	luaL_unref(mCurrentState, LUA_REGISTRYINDEX, mMethodID);
}

bool ScriptMethod::Invoke()
{
	int beforeCall = lua_gettop(mCurrentState);
	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mMethodID);
	if(lua_isfunction(mCurrentState, -1)) {
		lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mObjectID);
		if(lua_pcall(mCurrentState, 1, LUA_MULTRET, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			luaM_printerror(mCurrentState, err);
			lua_pop(mCurrentState, 1);
			return false;
		}
		mNumResults = lua_gettop(mCurrentState) - beforeCall;
		return true;
	}
	lua_pop(mCurrentState, 1);
	return false;
}

int32 ScriptMethod::GetInt32()
{
	if(mNumResults == 0)
		return 0;

	int32 result = lua_tointeger(mCurrentState, -1); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

uint32 ScriptMethod::GetUInt32()
{
	if(mNumResults == 0)
		return 0;

	uint32 result = lua_tointeger(mCurrentState, -1); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

float32 ScriptMethod::GetFloat32()
{
	if(mNumResults == 0)
		return 0.0f;

	float32 result = (float32)lua_tonumber(mCurrentState, -1); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

float64 ScriptMethod::GetFloat64()
{
	if(mNumResults == 0)
		return 0.0;

	float64 result = lua_tonumber(mCurrentState, -1); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

playstate::string ScriptMethod::GetString()
{
	if(mNumResults == 0)
		return playstate::string("");

	playstate::string result = lua_tostring(mCurrentState, -1); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

Scriptable* ScriptMethod::GetObject()
{
	if(mNumResults == 0)
		return NULL;

	Scriptable* result = luaM_getinstance(mCurrentState); lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}

bool ScriptMethod::GetBool()
{
	if(mNumResults == 0)
		return false;

	bool result = lua_toboolean(mCurrentState, -1) == 1; lua_pop(mCurrentState, 1); mNumResults--;
	return result;
}
