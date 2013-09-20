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
	return FindInt(playstate::string(key), defaultVal);
}

int32 ScriptCollection::FindInt(const playstate::string& key, int32 defaultVal) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			int32 result = (int32)lua_tonumber(mLuaState, -1);
			lua_pop(mLuaState, 1);
			return result;
		}
	}

	return defaultVal;
}

float32 ScriptCollection::FindFloat(const playstate::character* key, float32 defaultVal) const
{
	return FindFloat(playstate::string(key), defaultVal);
}

float32 ScriptCollection::FindFloat(const playstate::string& key, float32 defaultVal) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			float32 result = (float32)lua_tonumber(mLuaState, -1);
			lua_pop(mLuaState, 1);
			return result;
		}
	}

	return defaultVal;
}

bool ScriptCollection::FindBool(const playstate::character* key, bool defaultVal) const
{
	return FindBool(playstate::string(key), defaultVal);
}

bool ScriptCollection::FindBool(const playstate::string& key, bool defaultVal) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			bool result = lua_toboolean(mLuaState, -1) == 1;
			lua_pop(mLuaState, 1);
			return result;
		}
	}

	return defaultVal;
}

playstate::string ScriptCollection::FindString(const playstate::character* key, const playstate::character* defaultVal) const
{
	return FindString(playstate::string(key), defaultVal);
}

playstate::string ScriptCollection::FindString(const playstate::string& key, const playstate::character* defaultVal) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			playstate::string value = lua_tostring(mLuaState, -1);
			lua_pop(mLuaState, 1);
			return value;
		}
	}
	return defaultVal;
}

Color ScriptCollection::FindColor(const playstate::character* key, const Color& defaultColor) const
{
	return FindColor(playstate::string(key), defaultColor);
}

Color ScriptCollection::FindColor(const playstate::string& key, const Color& defaultColor) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			return luaM_popcolor(mLuaState);
		}
	}
	return defaultColor;
}

Vector2 ScriptCollection::FindVector2(const playstate::character* key, const Vector2& defaultVector) const
{
	return FindVector2(playstate::string(key), defaultVector);
}

Vector2 ScriptCollection::FindVector2(const playstate::string& key, const Vector2& defaultVector) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			return luaM_popvector2(mLuaState);
		}
	}
	return defaultVector;
}

ResourceData* ScriptCollection::FindResourceData(const playstate::string& key, ResourceData* defaultObject) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			return luaM_popresourcedata(mLuaState);
		}
	}
	return defaultObject;
}

Scriptable* ScriptCollection::FindScriptablePtr(const playstate::string& key, Scriptable* defaultObject) const
{
	std::vector<playstate::string> keys = Split(key, '>');
	std::vector<playstate::string>::size_type size = keys.size();
	for(uint32 i = 0; i < size; ++i) {
		if(FindKey(keys[i])) {
			Scriptable* result = luaM_getinstance(mLuaState); 
			lua_pop(mLuaState, 1);
			return result;
		}
	}
	return defaultObject;
}

bool ScriptCollection::FindKey(const playstate::string& key) const
{
	if(mLuaState == NULL)
		return false;

	lua_rawgeti(mLuaState, LUA_REGISTRYINDEX, mScriptRef);
	bool istable = lua_istable(mLuaState, -1);
	std::vector<playstate::string> parts = Split(key, '.');
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
