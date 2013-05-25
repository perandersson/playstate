#include "../../memory/memory.h"
#include "script_exception.h"

using playstate::ScriptException;

ScriptException::ScriptException(const char* function, const long line, const char* file)
	: Exception(function, line, file)
{
}

ScriptException::ScriptException(const char* function, const long line, const char* file, const char* message, ...)
	: Exception(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	SetMessage(message, arglist);
    va_end(arglist);
}

ScriptException::~ScriptException()
{
}
