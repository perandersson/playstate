#pragma once
#include "../gui_widget.h"
#include "../../font/font.h"

namespace playstate
{
	class GuiButtonWidget : public GuiWidget
	{
	public:
		GuiButtonWidget();
		virtual ~GuiButtonWidget();

		//
		// Set the button text with the supplied font. 
		void SetText(const playstate::string& text);

	// GuiWidget
	public:
		virtual const void BuildWidgetGeometry(GuiGeometryBuilder& builder);

	// GuiWidget
	protected:
		virtual void OnStyleChanged(const GuiStyle& style);

	private:
		Resource<Font> mFont;
		playstate::string mText;
		Color mFrontColor;
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
