#pragma once

#include "rendering_exception.h"

namespace playstate
{
	class GfxProgramException : public RenderingException
	{
	public:
		GfxProgramException(const char* function, const long line, const char* file);
		GfxProgramException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~GfxProgramException();
	};
}

