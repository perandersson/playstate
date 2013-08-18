#pragma once

#include "../gui_widget.h"

namespace playstate
{
	class GuiFrameWidget : public GuiWidget
	{
	public:
		GuiFrameWidget();
		GuiFrameWidget(CanvasGroup* group);
		virtual ~GuiFrameWidget();

		//
		// Sets this widgets title 
		void SetTitle(const playstate::string& title);

	// GuiWidget
	public:
		virtual const void BuildWidgetGeometry(GuiGeometryBuilder& builder) const;

	private:
		playstate::string mTitle;
	};

	// Script integration

	extern int GuiFrameWidget_Factory(lua_State* L);
	extern int GuiFrameWidget_SetTitle(lua_State* L);
	static luaL_Reg GuiFrameWidget_Methods[] = {
		{ LUA_CONSTRUCTOR, GuiFrameWidget_Factory },
		{ "SetTitle", GuiFrameWidget_SetTitle },
		{ NULL, NULL }
	};
}
