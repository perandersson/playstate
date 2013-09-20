#pragma once
#include "../gui_style.h"
#include "../gui_geometry_builder.h"
#include "../gui_mouse_state.h"

namespace playstate
{
	class Canvas;

	class ButtonControl
	{
	public:
		ButtonControl();
		~ButtonControl();

		//
		void SetStyle(const GuiStyle& style);

		//
		// Renders a button
		bool Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& text);

	private:
		Resource<Font> mFont;
		Color mButtonColors[2 * GuiMouseState::NUM_STATES]; // Top and bottom for all the mouse events
		Color mFontColors[GuiMouseState::NUM_STATES]; // Top and bottom for all the mouse events
	};

}
