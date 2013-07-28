#pragma once

#include "../types.h"
#include "../singleton.h"
#include "../filesystem/file_system.h"

#include "script.h"
#include "scriptable.h"

#include "exception/script_exception.h"
#include "exception/script_not_found_exception.h"

#include "../logging/logger_factory.h"

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
		ScriptSystem(IFileSystem& fileSystem, ILoggerFactory& loggerFactory);
		virtual ~ScriptSystem();

	public:
		//
		// Compile the supplied file and returns the result. This method will always return a result
		// unless an exception is thrown.
		// @param fileName The absolute filename of the script file.
		// @throw playstate::ScriptNotFoundException If the supplied script file wasn't found.
		// @throw playstate::ScriptException If the script compilation wasn't successfull.
		std::auto_ptr<Script> CompileFile(const std::string& fileName);

		//
		// Compile the supplied file and returns the result. This method will always return a result
		// unless an exception is thrown.
		// @param fileName The absolute filename of the script file.
		// @param module The module name where we want the file to be loaded into
		// @throw playstate::ScriptNotFoundException If the supplied script file wasn't found.
		// @throw playstate::ScriptException If the script compilation wasn't successfull.
		std::auto_ptr<Script> CompileFile(const std::string& fileName, const std::string& module);
	
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
		void SetGlobalVar(const char* name, lua_Number value);

		//
		// Sets a global value
		void SetGlobalVar(const char* name, const char* value);

		//
		//
		void HandleGC();

	private:
		IFileSystem& mFileSystem;
		ILogger& mLogger;
		lua_State* mLuaState;
	};
}
