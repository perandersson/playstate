#pragma once

#include "../types.h"
#include "../math/color.h"
#include "../math/vector2.h"
#include "../resources/resource.h"
#include "scriptable.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	//
	// Useful for reading tables that originates from lua. The table are accessed using a '.' delimiter.
	// Example: root.part.part2.value
	// 
	// It's also possible to stack hierarchial values. The priority for these values are separated using a '>' delimiter, 
	// where the most important value is first.
	// Example: value>root.part.value
	class ScriptCollection
	{
	public:
		ScriptCollection();
		ScriptCollection(lua_State* L, script_ref scriptRef);
		ScriptCollection(const ScriptCollection& s);
		virtual ~ScriptCollection();
		
		//
		//
		ScriptCollection& operator = (const ScriptCollection& s);

	public:
		int32 FindInt(const playstate::character* key, int32 defaultVal = 0) const;
		int32 FindInt(const playstate::string& key, int32 defaultValue = 0) const;

		float32 FindFloat(const playstate::character* key, float32 defaultVal = 0.0f) const;
		float32 FindFloat(const playstate::string& key, float32 defaultVal = 0.0f) const;

		bool FindBool(const playstate::character* key, bool defaultVal = false) const;
		bool FindBool(const playstate::string& key, bool defaultVal = false) const;

		playstate::string FindString(const playstate::character* key, const playstate::character* defaultVal = "") const;
		playstate::string FindString(const playstate::string& key, const playstate::character* defaultVal = "") const;
		
		//
		// Find a color inside this collection and returns the result. If the
		// color wasn't found then return the supplied default color instead
		//
		// @param key
		// @param defaultColor
		Color FindColor(const playstate::character* key, const Color& defaultColor) const;
		Color FindColor(const playstate::string& key, const Color& defaultColor) const;

		//
		// Find a vector inside this collection.
		Vector2 FindVector2(const playstate::character* key, const Vector2& defaultVector) const;
		Vector2 FindVector2(const playstate::string& key, const Vector2& defaultVector) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::character* key) const;
		
		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::string& key) const;
		
		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::character* key, Resource<T> defaultValue) const;
		
		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::string& key, Resource<T> defaultValue) const;

		//
		// Find a scriptable instance in this collectiom.
		template<class T>
		T* FindScriptable(const playstate::character* key) const;

	protected:
		//
		// Find a key and pushes it to the top of the stack
		//
		// @return TRUE if the key is found; FALSE otherwise
		// @remark If no key is found then no element is pushed onto the stack
		bool FindKey(const playstate::string& key) const;

		//
		// 
		ResourceData* FindResourceData(const playstate::string& key, ResourceData* defaultObject) const;

		Scriptable* FindScriptablePtr(const playstate::string& key, Scriptable* defaultObject) const;

	protected:
		script_ref mScriptRef;
		lua_State* mLuaState;
	};

	template<class T>
	Resource<T> ScriptCollection::FindResource(const playstate::character* key) const {
		return FindResource<T>(playstate::string(key));
	}
	
	template<class T>
	Resource<T> ScriptCollection::FindResource(const playstate::string& key) const {
		ResourceData* data = FindResourceData(key, NULL);
		if(data != NULL)
			return Resource<T>(data);
		else
			return Resource<T>();
	}
	
	template<class T>
	Resource<T> ScriptCollection::FindResource(const playstate::character* key, Resource<T> defaultValue) const
	{
		return FindResource<T>(playstate::string(key), defaultValue);
	}
	
	template<class T>
	Resource<T> ScriptCollection::FindResource(const playstate::string& key, Resource<T> defaultValue) const
	{
		ResourceData* data = FindResourceData(key, defaultValue.GetResourceData());
		if(data != NULL)
			return Resource<T>(data);
		else
			return Resource<T>();
	}

	template<class T>
	T* ScriptCollection::FindScriptable(const playstate::character* key) const {
		Scriptable* ptr = FindScriptablePtr(playstate::string(key), NULL);
		if(ptr != NULL)
			return dynamic_cast<T*>(ptr);
		else
			return NULL;
	}
}
