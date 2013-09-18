#pragma once

#include "../types.h"
#include "../singleton.h"
#include "../filesystem/file_system.h"

#include "script.h"
#include "scriptable.h"

#include "exception/script_exception.h"
#include "exception/script_not_found_exception.h"

#include "../logging/logger.h"

#include <queue>

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	//
	// 
	class ScriptSystem : public Singleton<ScriptSystem>
	{
	public:
		ScriptSystem(IFileSystem& fileSystem, ILogger& logger);
		virtual ~ScriptSystem();

	public:
		//
		// Compile the supplied file and returns the result. This method will always return a result
		// unless an exception is thrown.
		// @param fileName The absolute filename of the script file.
		// @throw playstate::ScriptNotFoundException If the supplied script file wasn't found.
		// @throw playstate::ScriptException If the script compilation wasn't successfull.
		std::auto_ptr<Script> CompileFile(const playstate::string& fileName);

		// 
		// Register a list of methods for the supplied class name
		// @param className
		// @param methods
		void RegisterType(const char* className, luaL_Reg* methods);
		
		//
		// Registers a list of global functions
		void RegisterFunctions(luaL_Reg* functions);
		
		//
		// Sets a global value
		void SetGlobalVar(const char* name, float64 value);

		//
		// Sets a global value
		void SetGlobalVar(const char* name, const char* value);
		
		//
		// Sets a global value
		void SetGlobalVar(const char* name, bool value);

		//
		//
		void HandleGC();

		//
		// Enables/Disables debug mode for the script system.
		void SetDebugMode(bool debugMode);

		//
		// 
		void PushIdentity(const playstate::string& identity);
		void PopIdentity();
		playstate::string GetIdentity() const;

	private:
		IFileSystem& mFileSystem;
		ILogger& mLogger;
		lua_State* mLuaState;
		std::queue<playstate::string> mIdentities;
	};
}
