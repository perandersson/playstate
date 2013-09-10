#include "../memory/memory.h"
#include "script_collection.h"
#include "../functions.h"
#include "luam.h"
using namespace playstate;

ScriptCollection::ScriptCollection()
	: mLuaState(NULL), mScriptRef(0)
{
}

ScriptCollection::ScriptCollection(lua_State* L, script_ref scriptRef)
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

int32 ScriptCollection::FindInt(const playstate::character* key, int32 defaultVal) const
{
	int32 result = defaultVal;
	if(FindKey(key)) {
		result = (int32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}

float32 ScriptCollection::FindFloat(const playstate::character* key, float32 defaultVal) const
{
	float32 result = defaultVal;
	if(FindKey(key)) {
		result = (float32)lua_tonumber(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}

bool ScriptCollection::FindBool(const playstate::character* key, bool defaultVal) const
{
	bool result = defaultVal;
	if(FindKey(key)) {
		result = lua_toboolean(mLuaState, -1) == 1;
		lua_pop(mLuaState, 1);
	}
	return result;
}

playstate::string ScriptCollection::FindString(const playstate::character* key, const playstate::character* defaultVal) const
{
	playstate::string result = defaultVal;
	if(FindKey(key)) {
		result = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}
	return result;
}


Color ScriptCollection::FindColor(const char* key, Color defaultColor) const
{
	Color result = defaultColor;
	if(FindKey(key)) {
		result = luaM_popcolor(mLuaState);
	}
	return result;
}

Vector2 ScriptCollection::FindVector2(const char* key, Vector2 defaultVector) const
{
	Vector2 result = defaultVector;
	if(FindKey(key)) {
		result = luaM_popvector2(mLuaState);
	}
	return result;
}

ResourceData* ScriptCollection::FindResourceData(const playstate::character* key, ResourceData* defaultObject) const
{
	ResourceData* result = defaultObject;
	if(FindKey(key)) {
		result = luaM_popresource(mLuaState);
	}
	return result;
}

Scriptable* ScriptCollection::FindScriptablePtr(const playstate::character* key, Scriptable* defaultObject) const
{
	Scriptable* result = defaultObject;
	if(FindKey(key)) {
		result = luaM_getinstance(mLuaState); 
		lua_pop(mLuaState, 1);
	}
	return result;
}

bool ScriptCollection::FindKey(const playstate::character* key) const
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
