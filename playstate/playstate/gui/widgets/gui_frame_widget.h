#pragma once

#include "../gui_widget.h"

namespace playstate
{
	class GuiFrameWidget : public GuiWidget
	{
	public:
		GuiFrameWidget(const std::string& title, float32 width, float32 height);
		GuiFrameWidget(CanvasGroup* group, const std::string& title, float32 width, float32 height);
		virtual ~GuiFrameWidget();

	// GuiWidget
	public:
		virtual const void BuildWidgetGeometry(GuiGeometryBuilder& builder) const;

	private:
		std::string mTitle;
	};

	// Script integration

	extern int GuiFrameWidget_Factory(lua_State* L);
	static luaL_Reg GuiFrameWidget_Methods[] = {
		{ LUA_CONSTRUCTOR, GuiFrameWidget_Factory },
		{ NULL, NULL }
	};
}
