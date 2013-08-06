#pragma once

#include "../resource_manager.h"
#include "../resource_changed_listener.h"
#include "../exception/load_resource_exception.h"
#include "../../thread/thread_factory.h"

#include <string>
#include <memory>

namespace playstate
{
	class RenderSystem;
	class IFileSystem;

	//
	// An implementation that supports multi-threaded resource loading- and unloading.
	// TODO Implement non-blocking locks.
	class ThreadedResourceManager : public IResourceManager, public IRunnable
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
		ThreadedResourceManager(RenderSystem& renderSystem, IFileSystem& fileSystem);
		~ThreadedResourceManager();
		
		//
		// Polls the resource loader thread for loaded resources. This method is used so that
		// we don't have to use locks every time we want to use a resource in the game engine.
		void Poll();

	// IResourceManager
	public:
		virtual void RegisterResourceType(IResourceLoader*, const std::string& suffix);
		virtual ResourceData* GetResourceData(const std::string& path);
		virtual void UnloadResourceData(ResourceData* data);

	// IRunnable
	public:
		virtual void Run(IThread& thread);

	private:
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
}
