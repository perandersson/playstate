#pragma once

#include "resource_data.h"

#include <list>
#include <algorithm>
#include <cassert>

namespace playstate
{
	class ResourceManager;

	template<class T>
	class Resource
	{
		friend class ResourceManager;

	public:
		Resource() : mData(0)
		{
		}

		Resource(ResourceData* data) : mData(data)
		{
			assert_not_null(data);
		}

		Resource(const Resource<T>& resource) : mData(resource.mData)
		{
		}

		template<class U>
		Resource(const Resource<U>& resource) : mData(0)
		{
			ResourceData* data = const_cast<Resource<U>&>(resource).GetResourceData();
			if(data != 0) {
				if(dynamic_cast<T*>(data->Resource)) {
					mData = data;
				}
			}
		}

		~Resource()
		{
		}

		T* Get()
		{
			if(IsNull())
				return 0;

			return static_cast<T*>(mData->Resource);
		}
	
		const T* Get() const
		{
			if(IsNull())
				return 0;

			return static_cast<const T*>(mData->Resource);
		}
	
		T* operator->()
		{
			if(IsNull())
				return 0;

			return static_cast<T*>(mData->Resource);
		}

		const T* operator->() const
		{
			if(IsNull())
				return 0;
		
			return static_cast<const T*>(mData->Resource);
		}

		bool IsLoaded() const
		{
			return mData != 0 && !mData->Unloaded && !mData->Loading;
		}

		bool IsNull() const
		{
			return mData == 0;
		}

		bool IsDefault() const
		{
			if(mData == 0)
				return false;

			return mData->DefaultResource == mData->Resource;
		}
	
		ResourceData* GetResourceData()
		{
			return mData;
		}

		void AddListener(IResourceChangedListener* listener)
		{
			// It's not possible to handle events such as OnLoaded for resources with no data.
			// i.e. Resource<Model> model; // No assignment
			// model.AddListener(instance); // This will break the assert
			assert(mData != NULL && "You are trying to add a listener to a resource with no data attached to it.");
			mData->Listener.AddListener(listener);
		}

		void RemoveListener(IResourceChangedListener* listener)
		{
			// It's not possible to handle events such as OnLoaded for resources with no data.
			// i.e. Resource<Model> model; // No assignment
			// model.AddListener(instance); // This will break the assert
			assert(mData != NULL && "You are trying to remove a listener from a resource with no data attached to it.");
			mData->Listener.RemoveListener(listener);
		}

	private:
		ResourceObject* GetResourceObject()
		{
			if(mData == 0)
				return 0;

			if(mData->Resource == 0)
				return 0;

			if(mData->Resource == mData->DefaultResource)
				return 0;

			return mData->Resource;
		}

	protected:
		ResourceData* mData;
	};
}
