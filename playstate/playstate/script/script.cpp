#include "../memory/memory.h"
#include "script.h"
#include <stdarg.h>
using namespace playstate;

Script::Script(lua_State* L, uint32 numResults)
	: mLuaState(L), mNumResults(numResults)
{
	assert_not_null(L);
}

Script::~Script()
{
	if(mNumResults > 0) {
		lua_pop(mLuaState, (int32)mNumResults);
		mNumResults = 0;
	}

	mLuaState = NULL;
}

void Script::Evaluate(const char* fml, ...)
{
	char buffer[1024];
	va_list args;
	va_start(args, fml); 
	vsprintf(buffer, fml, args);
	va_end(args);

	Execute(buffer);
}

void Script::Execute(const char* expression)
{
	int result = luaL_dostring(mLuaState, expression);
	if(result != 0) 
	{
		// Might be error? or result? Assume error for now!
		std::string err = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
		THROW_EXCEPTION(ScriptException, "Could not evaluate expression:\n%s\n\nReason:\n%s", expression, err.c_str());
	}
}

ScriptCollection Script::ReadCollection()
{
	if(mNumResults == 0)
		THROW_EXCEPTION(ScriptException, "The script file didn't return any valid results");

	if(!lua_istable(mLuaState, -1)) {
		lua_pop(mLuaState, 1);
	}
	mNumResults--;

	int configRef = luaL_ref(mLuaState, LUA_REGISTRYINDEX);
	return ScriptCollection(mLuaState, configRef);
}
