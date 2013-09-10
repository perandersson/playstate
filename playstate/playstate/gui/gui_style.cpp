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

GuiStyle& GuiStyle::operator = (const GuiStyle& s)
{
	ScriptCollection::operator=(s);
	return *this;
}
