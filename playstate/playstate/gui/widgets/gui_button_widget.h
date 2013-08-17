#pragma once
#include "../gui_widget.h"

namespace playstate
{
	class GuiButtonWidget : public GuiWidget
	{
	public:
		GuiButtonWidget();
		GuiButtonWidget(CanvasGroup* group);
		virtual ~GuiButtonWidget();

		//
		// Set the button text
		void SetText(const std::string& text);

	// GuiWidget
	public:
		virtual const void BuildWidgetGeometry(GuiGeometryBuilder& builder) const;

	private:
		std::string mText;
	};

	// Script integration

	extern int GuiButtonWidget_Factory(lua_State* L);
	extern int GuiButtonWidget_SetText(lua_State* L);
	static luaL_Reg GuiButtonWidget_Methods[] = {
		{ LUA_CONSTRUCTOR, GuiButtonWidget_Factory },
		{ "SetText", GuiButtonWidget_SetText },
		{ NULL, NULL }
	};
}
