#include "../../memory/memory.h"
#include "threaded_resource_manager.h"
#include "../../filesystem/file_system.h"

#include <queue>

using namespace playstate;

ThreadedResourceManager::ThreadedResourceManager(RenderSystem& renderSystem, IFileSystem& fileSystem, ILogger& logger) 
	: mRenderSystem(renderSystem), mFileSystem(fileSystem), mLogger(logger), mThread(0)
{
	mLoadRequestLock = new SimpleLock();
	mLoadResponseLock = new SimpleLock();

	mThread = new RenderAwareThread(this);
	mThread->Start();
}

ThreadedResourceManager::~ThreadedResourceManager()
{
	if(mThread != NULL) {
		delete mThread;
		mThread = NULL;
	}

	if(mLoadResponseLock != NULL) {
		delete mLoadResponseLock;
		mLoadResponseLock = NULL;
	}

	if(mLoadRequestLock != NULL) {
		delete mLoadRequestLock;
		mLoadRequestLock = NULL;
	}

	LoadedResources::iterator rit = mLoadedResources.begin();
	LoadedResources::const_iterator rend = mLoadedResources.end();
	for(; rit != rend; ++rit) {
		if(rit->second->Resource != rit->second->DefaultResource) {
			if(rit->second->Resource != NULL) {
				delete rit->second->Resource;
			}
		}

		rit->second->Resource = NULL;
		rit->second->DefaultResource = NULL;
		delete rit->second;
		rit->second = NULL;
	}
	mLoadedResources.clear();

	Loaders::iterator loaderIt = mLoaders.begin();
	Loaders::iterator loaderEnd = mLoaders.end();
	for(; loaderIt != loaderEnd; ++loaderIt) {
		delete loaderIt->second;
	}

	mLoaders.clear();
}

ResourceData* ThreadedResourceManager::GetResourceData(const playstate::string& path)
{
	const playstate::string suffix = GetSuffixFromName(path);	
	IResourceLoader* loader = GetLoaderFromSuffix(suffix);
	if(loader == NULL) {
		return NULL;
	}

	LoadedResources::iterator it = mLoadedResources.find(path);
	if(it != mLoadedResources.end()) {
		const ResourceData* data = it->second;
		if(!data->Unloaded) {
			return it->second;
		}
	}

	// A resource can either be:
	// 1. Not loaded
	// 3. Unloaded
	
	// First check if the resource type is threadable. No need to resume the loader thread
	// if not necessary
	if(!loader->IsThreadable()) {
		ResourceData* data = NULL;
		if(it == mLoadedResources.end()) {
			data = LoadResourceAndWait(path, loader);
			mLoadedResources.insert(std::make_pair(path, data));
		} else {
			data = it->second;
			data->Resource = NULL;

			std::auto_ptr<IFile> file = mFileSystem.OpenFile(path);
			if(file->Exists())
				data->Resource = loader->Load(*file);
			else
				ILogger::Get().Error("Could not find resource: '%s'", file->GetPath().c_str());

			data->Unloaded = false;
			data->Loading = false;
		}
		return data;
	}

	// Only threadable unloaded or not loaded data here!
	ResourceData* data = NULL;
	if(it != mLoadedResources.end()) {
		// Unloaded but threadable
		data = it->second;
	} else {
		// Not loaded but threadable
		data = new ResourceData;
		mLoadedResources.insert(std::make_pair(path, data));
	}
	
	AddLoadRequest(path, loader, data);

	// Only notify thread if necessary
	mThread->Notify();
	
	return data;
}

playstate::string ThreadedResourceManager::GetSuffixFromName(const playstate::string& name) const
{
	playstate::string::size_type separator = name.find_last_of('.');
	assert(separator != playstate::string::npos && "No real filename was supplied");
	playstate::string suffix = name.substr(separator);
	return suffix;
}

ResourceData* ThreadedResourceManager::LoadResourceAndWait(const playstate::string& path, IResourceLoader* loader)
{
	std::auto_ptr<IFile> file = mFileSystem.OpenFile(path);
	ResourceObject* resource = NULL;
	if(file->Exists())
		resource = loader->Load(*file);

	ResourceData* data = new ResourceData;
	data->Loading = data->Unloaded = false;
	data->DefaultResource = loader->GetDefaultResource();
	data->Resource = resource;
	if(resource == NULL)
		data->Resource = data->DefaultResource;
	else
		data->Listener.OnLoaded(resource);

	return data;
}


void ThreadedResourceManager::RegisterResourceType(IResourceLoader* resourceLoader, const playstate::string& suffix)
{
	assert(mLoaders.find(suffix) == mLoaders.end() && "You are trying to add the same loader twice");
	mLoaders.insert(std::make_pair(suffix, resourceLoader));

	// Make sure that the resource loader has created the default resource if neccessary.
	if(resourceLoader->IsThreadable())
		resourceLoader->GetDefaultResource();
}

void ThreadedResourceManager::AddLoadRequest(const playstate::string& name, IResourceLoader* loader, ResourceData* data)
{
	data->Unloaded = false;
	data->Loading = true;
	data->DefaultResource = loader->GetDefaultResource();
	data->Resource = data->DefaultResource;

	LoadRequestResponse* request = new LoadRequestResponse();
	request->Name = name;
	request->Loader = loader;
	request->Data = data;
	request->LoadedResource = NULL;

	ScopedLock sl(mLoadRequestLock);
	mRequests.push_back(request);
}

void ThreadedResourceManager::GetAndClearLoadRequests(LoadRequests& target)
{
	if(mRequests.empty())
		return;

	ScopedLock sl(mLoadRequestLock);

	target = LoadRequests(mRequests);
	mRequests.clear();
}

void ThreadedResourceManager::GetAndClearLoadResponses(LoadResponses& target)
{
	if(mResponses.empty())
		return;

	ScopedLock sl(mLoadResponseLock);

	target = LoadResponses(mResponses);
	mResponses.clear();
}

void ThreadedResourceManager::UnloadResourceData(ResourceData* data)
{
	if(data == NULL)
		return;
	
	if(data->Resource != data->DefaultResource)
	{
		data->Listener.OnUnloading(data->Resource);
		delete data->Resource;
	}

	data->Resource = data->DefaultResource;
	data->Unloaded = true;
}

void ThreadedResourceManager::Poll()
{
	LoadResponses responses;
	GetAndClearLoadResponses(responses);
	if(!responses.empty()) {
		LoadResponses::size_type size = responses.size();
		for(LoadResponses::size_type i = 0; i < size; ++i) {
			LoadRequestResponse* response = responses[i];
			response->Data->Loading = false;
			if(response->LoadedResource != NULL) {
				response->Data->Resource = response->LoadedResource;
				response->Data->Listener.OnLoaded(response->LoadedResource);
			}
			delete response;
		}
	}
}

void ThreadedResourceManager::Run(IThread& thread)
{
	LoadRequests requests;
	ThreadState state = THREAD_STOPPED;
	while((state = thread.GetState()) == THREAD_RUNNING) {
		GetAndClearLoadRequests(requests);
		if(!requests.empty()) {
			LoadRequests::size_type size = requests.size();
			for(LoadRequests::size_type i = 0; i < size; ++i) {
				LoadRequestResponse* request = requests[i];
				std::auto_ptr<IFile> file = mFileSystem.OpenFile(request->Name);
				if(file->Exists()) {
					try {
						request->LoadedResource = request->Loader->Load(*file);
					} catch(ResourceException e) {
						mLogger.Error("Could not load resource: '%s'. Reason: '%s'", file->GetPath().c_str(),
							e.GetMessage().c_str());
					}
				} else {
					ILogger::Get().Error("Could not find resource: '%s'", request->Name.c_str());
				}
				ScopedLock scl(mLoadResponseLock);
				mResponses.push_back(request);
			}
			requests.clear();
			continue;
		}
		thread.Wait();
	}
}

IResourceLoader* ThreadedResourceManager::GetLoaderFromSuffix(const playstate::string& suffix) const
{
	Loaders::const_iterator it = mLoaders.find(suffix);
	if(it == mLoaders.end())
		return NULL;

	return it->second;
}
