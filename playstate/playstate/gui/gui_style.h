#pragma once
#include "../script/script_collection.h"
#include "../math/color.h"
#include "../math/vector2.h"
#include "../resources/resource.h"

namespace playstate
{
	class GuiStyle : public ScriptCollection
	{
	public:
		GuiStyle();
		GuiStyle(lua_State* L, uint32 scriptRef);
		GuiStyle(const GuiStyle& s);
		virtual ~GuiStyle();

		//
		// Find a color inside this style and returns the result. If the
		// color wasn't found then return the supplied default color instead
		//
		// @param key
		// @param defaultColor
		Color FindColor(const playstate::character* key, Color defaultColor) const;
		Vector2 FindVector2(const playstate::character* key, Vector2 defaultVector) const;

		//
		// Find a resource inside this style, for example a Font resource.
		template<class T>
		Resource<T> FindResource(const playstate::character* key) const;

		//
		//
		GuiStyle& operator = (const GuiStyle& s);

	private:
		ResourceData* FindResourceData(const playstate::character* key, ResourceData* defaultObject) const;
	};

	template<class T>
	Resource<T> GuiStyle::FindResource(const playstate::character* key) const {
		return Resource<T>(FindResourceData(key, NULL));
	}
}
