#include "../memory/memory.h"
#include "script_system.h"
#include "../kernel.h"
using namespace playstate;

template<> playstate::ScriptSystem* playstate::Singleton<playstate::ScriptSystem>::gSingleton = NULL;

int __playstate_delete(lua_State* L)
{
	int numobjects = lua_gettop(L);
	for(int i = 0; i < numobjects; ++i) {
		void* object = playstate::luaM_getinstance(L); 
		if(object != NULL) {
			Scriptable* ptr = reinterpret_cast<Scriptable*>(object);
			delete ptr;
			playstate::luaM_setinstance(L, 0);
		}
		lua_pop(L, 1);
	}

	return 0;
}

int __playstate_lua_require(lua_State* L)
{
	std::string package = lua_tostring(L, -1);
	lua_pop(L, 1);
		
	if(package.c_str()[0] != '/') {
		for(unsigned int i = 0; i < package.size(); ++i) {
			if(package[i] == '.') {
				package[i] = '/';
			}
		}
		package += ".lua";
		package = std::string("/") + package;
	}
	std::auto_ptr<IFile> file = IFileSystem::Get().OpenFile(package);

	if(file->Exists()) {
		std::string value = file->Read().str();
		int res = luaL_loadstring(L, value.c_str());
		if(res != 0) {
			std::string err = lua_tostring(L, -1);
			//Services::Get<ILoggerManager>().Error("Could not load file \"%s\". Reason: %s", err.c_str());
		}
	} else {
		lua_pushfstring(L, "\n\tCould not include file \"%s\". File not found", package.c_str());
	}
	return 1;
}

ScriptSystem::ScriptSystem(IFileSystem& fileSystem, ILogger& logger)
	: mFileSystem(fileSystem), mLogger(logger), mLuaState(NULL)
{
	mLuaState = luaL_newstate();
	luaL_openlibs(mLuaState);

	lua_register(mLuaState, "__playstate_lua_require", __playstate_lua_require);
	int res = luaL_dostring(mLuaState, "table.insert(package.searchers, __playstate_lua_require)");
	if(res != 0) {
		std::string err = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
	}

	lua_register(mLuaState, "Delete", __playstate_delete);

}

ScriptSystem::~ScriptSystem()
{
	if(mLuaState != NULL) {
		//assert(lua_gettop(mLuaState) == 0 && "Lua stack is not 0 which means that we have a memory leak somewhere");
		lua_close(mLuaState);
		mLuaState = NULL;
	}
}

std::auto_ptr<Script> ScriptSystem::CompileFile(const std::string& fileName)
{
	return CompileFile(fileName, fileName);
}

std::auto_ptr<Script> ScriptSystem::CompileFile(const std::string& fileName, const std::string& module)
{
	std::auto_ptr<IFile> scriptFile = mFileSystem.OpenFile(fileName);
	if(!scriptFile->Exists()) {
		mLogger.Error("Could not find script file: '%s'", fileName.c_str());
		THROW_EXCEPTION(ScriptNotFoundException, "Could not find script file: '%s'", fileName.c_str());
	}

	uint32 numResults = lua_gettop(mLuaState);
	std::string value = scriptFile->Read().str();
	int res = luaL_loadstring(mLuaState, value.c_str());
	if(res != 0) {
		std::string err = lua_tostring(mLuaState, -1);
		lua_pop(mLuaState, 1);
		mLogger.Error("Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
		THROW_EXCEPTION(ScriptException, "Could not load file: %s. Reason: %s", fileName.c_str(), err.c_str());
	} else {
		res = lua_pcall(mLuaState, 0, LUA_MULTRET, NULL);
		if(res != 0) {
			std::string err = lua_tostring(mLuaState, -1);
			lua_pop(mLuaState, 1);
			mLogger.Error("Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
			THROW_EXCEPTION(ScriptException, "Could not compile file: %s. Reason: %s", fileName.c_str(), err.c_str());
		}
		numResults = lua_gettop(mLuaState) - numResults;
	}


	Script* cs = new Script(mLuaState, numResults);
	return std::auto_ptr<Script>(cs);
}

void ScriptSystem::RegisterType(const char* className, luaL_Reg* methods)
{
	assert_not_null(className);
	assert_not_null(methods);

	const std::string name(className);
	const std::string metaName = "playstate." + name;

	luaL_newmetatable(mLuaState, metaName.c_str());
	lua_pushvalue(mLuaState, -1);
	lua_setfield(mLuaState, -2, "__index");
	luaL_setfuncs(mLuaState, methods, 0);
	lua_pop(mLuaState, 1);
	/*
	if(methods != 0) {
		luaL_getmetatable(mLuaState, metaName.c_str());
		lua_pop(mLuaState, 1);
	}
	*/
	lua_newtable(mLuaState);
	luaL_getmetatable(mLuaState, metaName.c_str());
	lua_setmetatable(mLuaState, -2);
	if(methods != NULL) {
		luaL_setfuncs(mLuaState, methods, 0);
	}
	lua_setglobal(mLuaState, className);
}

void ScriptSystem::RegisterFunctions(luaL_Reg* functions)
{
	assert_not_null(functions);

	for(; functions->name != NULL; functions++) {
		lua_register(mLuaState, functions->name, functions->func);
	}
}

void ScriptSystem::SetGlobalVar(const char* name, lua_Number value)
{
	lua_pushnumber(mLuaState, value);
	lua_setglobal(mLuaState, name);
}

void ScriptSystem::SetGlobalVar(const char* name, const char* value)
{
	lua_pushstring(mLuaState, value);
	lua_setglobal(mLuaState, name);
}

void ScriptSystem::HandleGC()
{
	lua_gc(mLuaState, LUA_GCSTEP, 180);
	lua_gc(mLuaState, LUA_GCSTOP, 0);
}

