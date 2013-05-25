#pragma once

#include "../script/scriptable.h"
#include "resource_object.h"
#include "resource_changed_listener.h"
#include "../types.h"

#include <list>

namespace playstate
{
	class ResourceDataListener : public IResourceChangedListener
	{
	public:
		ResourceDataListener();
		virtual ~ResourceDataListener();

		void AddListener(IResourceChangedListener* listener);
		void RemoveListener(IResourceChangedListener* listener);

	// IResourceChangedListener
	public:
		virtual void OnLoaded(ResourceObject* object);
		virtual void OnUnloading(ResourceObject* object);

	private:
		std::list<IResourceChangedListener*> mListeners;
	};
	
	class ResourceData : public Scriptable
	{
	public:
		ResourceData() {}
		~ResourceData() {}

		ResourceObject* Resource;
		ResourceObject* DefaultResource;
		bool Loading;
		bool Unloaded;
		ResourceDataListener Listener;
	};

}

