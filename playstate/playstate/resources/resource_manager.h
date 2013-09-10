#pragma once

#include "../singleton.h"
#include "resource.h"
#include "resource_loader.h"

namespace playstate
{
	//
	// Service for managing resource loading- and unloading. 
	class IResourceManager : public Singleton<IResourceManager>
	{
	public:
		//
		// Method for retrieving a resource at the supplied path. 
		//
		// @return A smart resource container. If the supplied path is not found then a resource wrapper containing the default resource
		//	will be returned instead.
		template<class T>
		Resource<T> GetResource(const char* path);

		//
		// Method for retrieving a resource at the supplied path. 
		//
		// @return A smart resource container. If the supplied path is not found then a resource wrapper containing the default resource
		//	will be returned instead.
		template<class T>
		Resource<T> GetResource(const playstate::string& path);

		//
		// Method for unloading the supplied resource. The internal resource data will be removed and the supplied containers resource will be
		// set to the default resource instead. This is ensure that a resource is always available.
		//
		// @param resource.
		template<class T>
		void UnloadResource(Resource<T>& resource);

		//
		// Registers a resource loader instance  which is used when trying to load a resource ending with
		// the supplied suffix. 
		//
		// @param resourceLoader
		// @param suffix
		virtual void RegisterResourceType(IResourceLoader*, const playstate::string& suffix) = 0;

		//
		// Non-public method for actually retrieving the raw resource data.
		//
		// @param path The resource path
		// @return An resource data object. This method always returns a valid, usable instance.
		virtual ResourceData* GetResourceData(const playstate::string& path) = 0;

		//
		// Non-public method for unloading a resource data object.
		// This will cause the instance to be marked as "unloaded". The ResourceObject instance will be reverted into the 
		// default instance until it's loaded again.
		//
		// @param data The object we want to remove.
		virtual void UnloadResourceData(ResourceData* data) = 0;
	};

	template<class T>
	Resource<T> IResourceManager::GetResource(const char* path) {
		return Resource<T>(GetResourceData(playstate::string(path)));
	}

	template<class T>
	Resource<T> IResourceManager::GetResource(const playstate::string& path) {
		return Resource<T>(GetResourceData(path));
	}

	template<class T>
	void IResourceManager::UnloadResource(Resource<T>& resource) {
		UnloadResourceData(resource.GetResourceData());
	}
	
	//
	// Script integration
	//

	extern int Resource_Load(lua_State* L);
	static luaL_Reg Resource_Methods[] = {
		{ "Load", Resource_Load },
		{ NULL, NULL }
	};
}
