#pragma once

#include "../singleton.h"
#include "resource.h"
#include "resource_changed_listener.h"
#include "resource_loader.h"
#include "exception/load_resource_exception.h"
#include "../thread/thread_factory.h"

#include <string>
#include <memory>

namespace playstate
{
	class RenderSystem;
	class IFileSystem;

	//
	// Base class for resource management.
	// TODO Make sure that the resource manager can take an IFile.
	class ResourceManager : public Singleton<ResourceManager>, public IRunnable
	{
		struct LoadRequestResponse
		{
			std::string Name;
			IResourceLoader* Loader;
			ResourceData* Data;
			ResourceObject* LoadedResource;
		};

		typedef std::hash_map<std::string, IResourceLoader*> Loaders;
		typedef std::hash_map<std::string, ResourceData*> LoadedResources;
		typedef std::vector<LoadRequestResponse*> LoadRequests;
		typedef std::vector<LoadRequestResponse*> LoadResponses;

	public:
		ResourceManager(RenderSystem& renderSystem, IFileSystem& fileSystem);
		~ResourceManager();

		//
		// Method for retrieving a resource at the supplied path. 
		//
		// @throws LoadResourceException thrown if the supplied resource needed to be loaded in this thread but the
		//		load procedure failed.
		// @throws ResourceException
		template<class T>
		Resource<T> GetResource(const char* path);

		//
		// Method for retrieving a resource at the supplied path. 
		//
		// @throws LoadResourceException thrown if the supplied resource needed to be loaded in this thread but the
		//		load procedure failed.
		// @throws ResourceException
		template<class T>
		Resource<T> GetResource(const std::string& path);

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
		void RegisterResourceType(IResourceLoader*, const std::string& suffix);

		//
		// Polls the resource manager for loaded resources.
		void Poll();

	// IRunnable
	public:
		virtual void Run(IThread& thread);

	private:
		//
		// Non-public method for actually retrieving the raw resource data.
		// @param path The resource path
		// @return An resource data object. This method always returns a valid, usable instance.
		ResourceData* GetResourceData(const std::string& path);

		//
		// Non-public method for unloading a resource data object.
		// This will cause the instance to be marked as "unloaded". The ResourceObject instance will be reverted into the 
		// default instance until it's loaded again.
		//
		// @param data The object we want to remove.
		void UnloadResourceData(ResourceData* data);
		
		IResourceLoader* GetLoaderFromSuffix(const std::string& suffix) const;
		std::string GetSuffixFromName(const std::string& name) const;
		void AddLoadRequest(const std::string& name, IResourceLoader* loader, ResourceData* data);
		ResourceData* LoadResourceAndWait(const std::string& name, IResourceLoader* loader);
		void GetAndClearLoadRequests(LoadRequests& target);
		void GetAndClearLoadResponses(LoadResponses& target);

	private:
		RenderSystem& mRenderSystem;
		IFileSystem& mFileSystem;

	private:
		RenderAwareThread* mThread;

		SimpleLock* mLoadResponseLock;
		SimpleLock* mLoadRequestLock;

		LoadRequests mRequests;
		LoadResponses mResponses;

		Loaders mLoaders;
		LoadedResources mLoadedResources;
	};
	
	template<class T>
	Resource<T> ResourceManager::GetResource(const char* path) {
		return Resource<T>(GetResourceData(std::string(path)));
	}

	template<class T>
	Resource<T> ResourceManager::GetResource(const std::string& path) {
		return Resource<T>(GetResourceData(path));
	}

	template<class T>
	void ResourceManager::UnloadResource(Resource<T>& resource) {
		UnloadResourceData(resource.GetResourceData());
	}
}
