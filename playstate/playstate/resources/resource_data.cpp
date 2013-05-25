#include "../memory/memory.h"
#include "resource_data.h"
#include <algorithm>
using namespace playstate;

ResourceDataListener::ResourceDataListener()
{
}

ResourceDataListener::~ResourceDataListener()
{
}

void ResourceDataListener::AddListener(IResourceChangedListener* listener)
{
	mListeners.push_back(listener);
}

void ResourceDataListener::RemoveListener(IResourceChangedListener* listener)
{
	std::list<IResourceChangedListener*>::iterator it = std::find(mListeners.begin(), mListeners.end(), listener);
	if(it != mListeners.end())
		mListeners.erase(it);
}

void ResourceDataListener::OnLoaded(ResourceObject* object)
{
	std::list<IResourceChangedListener*>::iterator it = mListeners.begin();
	std::list<IResourceChangedListener*>::const_iterator end = mListeners.end();
	for(; it != end; ++it) {
		(*it)->OnLoaded(object);
	}
}

void ResourceDataListener::OnUnloading(ResourceObject* object)
{
	std::list<IResourceChangedListener*>::iterator it = mListeners.begin();
	std::list<IResourceChangedListener*>::const_iterator end = mListeners.end();
	for(; it != end; ++it) {
		(*it)->OnUnloading(object);
	}
}
