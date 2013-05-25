#include "../memory/memory.h"
#include "scriptable.h"
using namespace playstate;

Scriptable::Scriptable()
	: mCurrentState(NULL), mScriptRef(0)
{
}

Scriptable::~Scriptable()
{
	uint32 size = mMethodIds.size();
	if(size > 0) {
		for(uint32 i = 0; i < size; ++i) {
			luaL_unref(mCurrentState, LUA_REGISTRYINDEX, mMethodIds[i]);
		}
		mMethodIds.clear();
	}

	if(mScriptRef != 0) {
		luaL_unref(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
		mScriptRef = 0;
		mCurrentState = NULL;
	}
}

bool Scriptable::RegisterObject(lua_State* L, uint32 id)
{
	mCurrentState = L;
	mScriptRef = id;
	this->OnRegistered();
	return true;
}

bool Scriptable::IsScriptBound() const
{
	return mCurrentState != NULL;
}

const uint32 Scriptable::GetID() const
{
	return mScriptRef;
}

bool Scriptable::PrepareMethod(const char* method)
{
	assert_not_null(method);
	assert(mScriptRef != 0 && "This object is not bound to script.");

	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
	if(lua_istable(mCurrentState, -1)) {
		lua_getfield(mCurrentState, -1, method);
		if(lua_isfunction(mCurrentState, -1)) {
			lua_remove(mCurrentState, -2); // Only the reference to the method exists on the stack after this
			lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
			return true;
		}
		lua_pop(mCurrentState, 1);
	}
	lua_pop(mCurrentState, 1);
	return false;
}

bool Scriptable::PrepareMethod(uint32 methodID)
{
	assert(methodID != 0 && "Invalid methodID");
	assert(mScriptRef != 0 && "This object is not bound to script.");

	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, methodID);
	if(lua_isfunction(mCurrentState, -1)) {
		lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
		return true;
	}
	lua_pop(mCurrentState, 1);
	return false;
}

uint32 Scriptable::GetMethodID(const char* method)
{
	assert_not_null(method);
	assert(mScriptRef != 0 && "This object is not bound to script.");

	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
	if(lua_istable(mCurrentState, -1)) {
		lua_getfield(mCurrentState, -1, method);
		if(lua_isfunction(mCurrentState, -1)) {
			lua_remove(mCurrentState, -2); // Only the reference to the method exists on the stack after this
			const uint32 ref = luaL_ref(mCurrentState, LUA_REGISTRYINDEX);
			mMethodIds.push_back(ref);
			return ref;
		}
		lua_pop(mCurrentState, 1);
	}
	lua_pop(mCurrentState, 1);
	return 0;
}

void Scriptable::OnRegistered()
{
}