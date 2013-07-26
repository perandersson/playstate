#pragma once

namespace playstate
{
	class ResourceObject;

	//
	// Interface for callbacks called when a resource is loaded or unloaded.
	class IResourceChangedListener
	{
	public:
		//
		// Method called when the supplied resource object is loaded.
		// @remark This method is always called in the main thread.
		// @param object The loaded object
		virtual void OnLoaded(ResourceObject* object) = 0;

		//
		// Method called whne the supplied resource is unloading
		// @param object The object that's being unloaded
		virtual void OnUnloading(ResourceObject* object) = 0;
	};

	//
	// Interface for listening on message callbacks when a collection of items are loaded
	class IResourceCollectionLoadedCallback
	{
	public:
		//
		// Method called when a collection of items has been loaded.
		virtual void OnLoaded() = 0;
	};

	//
	// The default resource changed listener class. This adds itself as a listener in the constructor
	// and then removes itself on destruction or if the remove listener method is invoked
	//class DefaultResourceChangedListener : public IResourceChangedListener
	//{
	//};
}

