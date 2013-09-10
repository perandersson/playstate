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
		float32 FindFloat(const playstate::character* key, float32 defaultVal = 0.0f) const;
		bool FindBool(const playstate::character* key, bool defaultVal = false) const;
		playstate::string FindString(const playstate::character* key, const playstate::character* defaultVal = "") const;
		
		//
		// Find a color inside this collection and returns the result. If the
		// color wasn't found then return the supplied default color instead
		//
		// @param key
		// @param defaultColor
		Color FindColor(const playstate::character* key, Color defaultColor) const;

		//
		// Find a vector inside this collection.
		Vector2 FindVector2(const playstate::character* key, Vector2 defaultVector) const;

		//
		// Find a resource inside this collection, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::character* key) const;

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
		bool FindKey(const playstate::character* key) const;

		//
		// 
		ResourceData* FindResourceData(const playstate::character* key, ResourceData* defaultObject) const;

		Scriptable* FindScriptablePtr(const playstate::character* key, Scriptable* defaultObject) const;

	protected:
		script_ref mScriptRef;
		lua_State* mLuaState;
	};

	template<class T>
	Resource<T> ScriptCollection::FindResource(const playstate::character* key) const {
		ResourceData* data = FindResourceData(key, NULL);
		if(data != NULL)
			return Resource<T>(data);
		else
			return Resource<T>();
	}
	
	template<class T>
	T* ScriptCollection::FindScriptable(const playstate::character* key) const {
		Scriptable* ptr = FindScriptablePtr(key, NULL);
		if(ptr != NULL)
			return dynamic_cast<T*>(ptr);
		else
			return NULL;
	}
}
