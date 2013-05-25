#pragma once

#include "../../exception.h"

namespace playstate
{
	class RenderingException : public Exception
	{
	public:
		RenderingException(const char* function, const long line, const char* file);
		RenderingException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~RenderingException();
	};
}
