#pragma once

#include "script_exception.h"

namespace playstate
{
	class ScriptNotFoundException : public ScriptException
	{
	public:
		ScriptNotFoundException(const char* function, const long line, const char* file);
		ScriptNotFoundException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~ScriptNotFoundException();
	};
}
