#include "../memory/memory.h"
#include "scriptable.h"
using namespace playstate;

Scriptable::Scriptable()
	: mCurrentState(NULL), mScriptRef(0)
{
}

Scriptable::~Scriptable()
{
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

ScriptMethod* Scriptable::GetMethod(const char* name)
{
	lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, mScriptRef);
	if(lua_istable(mCurrentState, -1)) {
		lua_getfield(mCurrentState, -1, name);
		if(lua_isfunction(mCurrentState, -1)) {
			lua_remove(mCurrentState, -2); // Only the reference to the method exists on the stack after this
			const uint32 ref = luaL_ref(mCurrentState, LUA_REGISTRYINDEX);
			return new ScriptMethod(mScriptRef, ref, mCurrentState);
		}
		lua_pop(mCurrentState, 1);
	}
	lua_pop(mCurrentState, 1);
	return NULL;
}

void Scriptable::OnRegistered()
{
}
