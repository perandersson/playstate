#pragma once

#include "../gui_widget.h"
#include "../../font/font.h"

namespace playstate
{
	class GuiFrameWidget : public GuiWidget
	{
	public:
		GuiFrameWidget();
		virtual ~GuiFrameWidget();

		//
		// Sets this widgets title 
		void SetTitle(const playstate::string& title);

	// GuiWidget
	protected:
		virtual void PreRender(GuiGeometryBuilder& builder);
		virtual void OnStyleChanged(const GuiStyle& style);

	private:
		playstate::string mTitle;
		Color mFrontColor;
		Resource<Font> mFont;
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
