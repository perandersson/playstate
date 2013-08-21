#pragma once

#include "../types.h"
#include "../filesystem/file.h"
#include "exception/load_resource_exception.h"

namespace playstate
{
	class ResourceObject;

	//
	// Interface for ....
	class IResourceLoader
	{
	public:
		virtual ~IResourceLoader() {}

		//
		// Method called when this resource type is loading it's resource. 
		// If this method is called then the file is guaranteeded to exist.
		// @param file 
		// @throws playstate::LoadResourceException
		virtual ResourceObject* Load(IFile& file) = 0;

		//
		// @return The default resource object. Can return NULL if the loader is non-threadable.
		virtual ResourceObject* GetDefaultResource() = 0;

		//
		// @return TRUE if this resource loader can load resources in another thread (or if it's
		//		acceptable to have a "default" resource while this resource is loading); FALSE otherwise.
		virtual bool IsThreadable() const = 0;
	};
}
