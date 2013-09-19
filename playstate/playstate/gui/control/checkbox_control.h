#pragma once
#include "../gui_style.h"
#include "../gui_geometry_builder.h"
#include "../gui_mouse_state.h"

namespace playstate
{
	class Canvas;

	class CheckboxControl
	{
	public:
		CheckboxControl();
		~CheckboxControl();

		//
		void SetStyle(const GuiStyle& style);

		//
		// Renders a slider based on the supplied properties
		bool Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, bool toggled, const playstate::string& text);

	private:
		Resource<Font> mFont;
		Color mFontColors[2]; // Unchecked, Checked

		uint32 mCheckboxSize;
		Color mCheckboxColors[2 * GuiMouseState::NUM_STATES]; // Top and bottom for all the mouse events
	};

}
