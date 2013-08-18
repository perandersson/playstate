#include "../memory/memory.h"
#include "script_collection.h"
#include "../functions.h"
using namespace playstate;

ScriptCollection::ScriptCollection()
	: mLuaState(NULL), mScriptRef(0)
{
}

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
	if(mLuaState != NULL) {
		luaL_unref(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
		mLuaState = NULL;
		mScriptRef = 0;
	}
}

int32 ScriptCollection::FindInt(const char* key, int32 defaultVal) const
{
	int32 result = defaultVal;
	if(FindKey(key)) {
		result = (int32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}

float32 ScriptCollection::FindFloat(const char* key, float32 defaultVal) const
{
	float32 result = defaultVal;
	if(FindKey(key)) {
		result = (float32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}

bool ScriptCollection::FindBool(const char* key, bool defaultVal) const
{
	bool result = defaultVal;
	if(FindKey(key)) {
		result = lua_toboolean(mLuaState, -1) == 1;
		lua_pop(mLuaState, 1);
	}
	return result;
}

playstate::string ScriptCollection::FindString(const char* key, const char* defaultVal) const
{
	playstate::string result = defaultVal;
	if(FindKey(key)) {
		result = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}

bool ScriptCollection::FindKey(const char* key) const
{
	if(mLuaState == NULL)
		return false;

	lua_rawgeti(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
	bool istable = lua_istable(mLuaState, -1);
	std::vector<playstate::string> parts = Split(playstate::string(key), '.');
	for(uint32 i = 0; i < parts.size(); ++i) {
		const playstate::string& part = parts[i];
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

ScriptCollection& ScriptCollection::operator = (const ScriptCollection& s)
{
	lua_State* newstate = s.mLuaState;
	lua_rawgeti(newstate, LUA_REGISTRYINDEX, s.mScriptRef);
	uint32 newref = luaL_ref(newstate, LUA_REGISTRYINDEX);

	if(mLuaState) {
		luaL_unref(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
		mLuaState = NULL;
		mScriptRef = 0;
	}

	mLuaState = newstate;
	mScriptRef = newref;
	return *this;
}
