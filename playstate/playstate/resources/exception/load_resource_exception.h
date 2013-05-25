#pragma once

#include "resource_exception.h"

namespace playstate
{
	class LoadResourceException : public ResourceException
	{
	public:
		LoadResourceException(const char* function, const long line, const char* file);
		LoadResourceException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~LoadResourceException();
	};
}
