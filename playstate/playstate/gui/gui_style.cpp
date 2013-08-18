#include "../memory/memory.h"
#include "gui_style.h"
#include "../script/luam.h"
using namespace playstate;

GuiStyle::GuiStyle()
	: ScriptCollection()
{
}

GuiStyle::GuiStyle(lua_State* L, uint32 scriptRef)
	: ScriptCollection(L, scriptRef)
{
}

GuiStyle::GuiStyle(const GuiStyle& s)
	: ScriptCollection(s)
{
}

GuiStyle::~GuiStyle()
{
}

Color GuiStyle::FindColor(const char* key, Color defaultColor) const
{
	Color result = defaultColor;
	if(FindKey(key)) {
		result = luaM_popcolor(mLuaState);
	}
	return result;
}

Vector2 GuiStyle::FindVector2(const char* key, Vector2 defaultVector) const
{
	Vector2 result = defaultVector;
	if(FindKey(key)) {
		result = luaM_popvector2(mLuaState);
	}
	return result;
}

GuiStyle& GuiStyle::operator = (const GuiStyle& s)
{
	ScriptCollection::operator=(s);
	return *this;
}
