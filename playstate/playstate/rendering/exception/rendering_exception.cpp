#include "../../memory/memory.h"
#include "rendering_exception.h"

using namespace playstate;


RenderingException::RenderingException(const char* function, const long line, const char* file)
	: Exception(function, line, file)
{
}

RenderingException::RenderingException(const char* function, const long line, const char* file, const char* message, ...)
	: Exception(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	SetMessage(message, arglist);
    va_end(arglist);
}

RenderingException::~RenderingException()
{
}
