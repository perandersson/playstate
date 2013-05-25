#include "../memory/memory.h"
#include "script_collection.h"
#include "../functions.h"
using namespace playstate;

ScriptCollection::ScriptCollection(lua_State* L, uint32 scriptRef)
	: mLuaState(L), mScriptRef(scriptRef)
{
}

ScriptCollection::ScriptCollection(const ScriptCollection& s)
	: mLuaState(s.mLuaState), mScriptRef(0)
{
	lua_rawgeti(mLuaState, LUA_REGISTRYINDEX, s.mScriptRef);
	mScriptRef = luaL_ref(mLuaState, LUA_REGISTRYINDEX);
}

ScriptCollection::~ScriptCollection()
{
	luaL_unref(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
	mLuaState = NULL;
	mScriptRef = 0;
}

int32 ScriptCollection::FindInt(const char* key, int32 defaultVal) const
{
#ifdef _DEBUG
	int begin = lua_gettop(mLuaState);
#endif

	int32 result = defaultVal;
	if(FindKey(key)) {
		result = (int32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}

#ifdef _DEBUG
	int end = lua_gettop(mLuaState);
	assert(begin == end && "Lua State is not valid");
#endif
	
	return result;
}

float32 ScriptCollection::FindFloat(const char* key, float32 defaultVal) const
{
#ifdef _DEBUG
	int begin = lua_gettop(mLuaState);
#endif

	float32 result = defaultVal;
	if(FindKey(key)) {
		result = (float32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}

#ifdef _DEBUG
	int end = lua_gettop(mLuaState);
	assert(begin == end && "Lua State is not valid");
#endif
	
	return result;
}

bool ScriptCollection::FindBool(const char* key, bool defaultVal) const
{
#ifdef _DEBUG
	int begin = lua_gettop(mLuaState);
#endif

	bool result = defaultVal;
	if(FindKey(key)) {
		result = lua_toboolean(mLuaState, -1) == 1;
		lua_pop(mLuaState, 1);
	}

#ifdef _DEBUG
	int end = lua_gettop(mLuaState);
	assert(begin == end && "Lua State is not valid");
#endif
	
	return result;
}

std::string ScriptCollection::FindString(const char* key, const char* defaultVal) const
{
#ifdef _DEBUG
	int begin = lua_gettop(mLuaState);
#endif

	std::string result = defaultVal;
	if(FindKey(key)) {
		result = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}

#ifdef _DEBUG
	int end = lua_gettop(mLuaState);
	assert(begin == end && "Lua State is not valid");
#endif
	
	return result;
}

bool ScriptCollection::FindKey(const char* key) const
{
	lua_rawgeti(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
	std::vector<std::string> parts = Split(std::string(key), '.');
	for(uint32 i = 0; i < parts.size(); ++i) {
		const std::string& part = parts[i];
		lua_pushstring(mLuaState, part.c_str());
		lua_gettable(mLuaState, -2);
		if(lua_isnoneornil(mLuaState, -1)) {
			lua_pop(mLuaState, 2);
			return false;
		}
		lua_remove(mLuaState, -2);
	}

	return true;
}
