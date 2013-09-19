#pragma once
#include "../gui_style.h"
#include "../gui_geometry_builder.h"
#include "../gui_mouse_state.h"

namespace playstate
{
	class Canvas;

	class LabelControl
	{
	public:
		LabelControl();
		~LabelControl();

		//
		void SetStyle(const GuiStyle& style);

		//
		// Renders a label based on the supplied parameters
		void Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& text);

	private:
		Resource<Font> mFont;
		Color mFontColor;
	};

}
