#pragma once

#include "resource_exception.h"

namespace playstate
{
	//
	// Exception thrown if an error occured while loading a resource. If the resource loader is threadable, then the exception handling
	// is handled by the ResourceManager. 
	class LoadResourceException : public ResourceException
	{
	public:
		LoadResourceException(const char* function, const long line, const char* file);
		LoadResourceException(const char* function, const long line, const char* file, const char* message, ...);
		virtual ~LoadResourceException();
	};
}
