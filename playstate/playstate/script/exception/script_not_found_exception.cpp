#include "../../memory/memory.h"
#include "script_not_found_exception.h"

using playstate::ScriptException;
using playstate::ScriptNotFoundException;

ScriptNotFoundException::ScriptNotFoundException(const char* function, const long line, const char* file)
	: ScriptException(function, line, file)
{
}

ScriptNotFoundException::ScriptNotFoundException(const char* function, const long line, const char* file, const char* message, ...)
	: ScriptException(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	SetMessage(message, arglist);
    va_end(arglist);
}

ScriptNotFoundException::~ScriptNotFoundException()
{
}
