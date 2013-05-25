#include "../../memory/memory.h"
#include "load_resource_exception.h"
using namespace playstate;


LoadResourceException::LoadResourceException(const char* function, const long line, const char* file)
	: ResourceException(function, line, file)
{
}

LoadResourceException::LoadResourceException(const char* function, const long line, const char* file, const char* message, ...)
	: ResourceException(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	SetMessage(message, arglist);
    va_end(arglist);
}

LoadResourceException::~LoadResourceException()
{
}
