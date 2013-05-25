#include "../../memory/memory.h"
#include "gfx_program_exception.h"

using namespace playstate;


GfxProgramException::GfxProgramException(const char* function, const long line, const char* file)
	: RenderingException(function, line, file)
{
}

GfxProgramException::GfxProgramException(const char* function, const long line, const char* file, const char* message, ...)
	: RenderingException(function, line, file)
{
	va_list arglist;
    va_start(arglist, message);
	SetMessage(message, arglist);
    va_end(arglist);
}

GfxProgramException::~GfxProgramException()
{
}
