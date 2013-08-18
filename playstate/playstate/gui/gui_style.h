#pragma once
#include "../script/script_collection.h"
#include "../math/color.h"
#include "../math/vector2.h"

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
		Color FindColor(const char* key, Color defaultColor) const;
		Vector2 FindVector2(const char* key, Vector2 defaultVector) const;

		//
		//
		GuiStyle& operator = (const GuiStyle& s);

	private:
	};
}
