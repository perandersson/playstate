#pragma once
#include "../gui_style.h"
#include "../gui_geometry_builder.h"
#include "../gui_mouse_state.h"

namespace playstate
{
	class Canvas;

	class FrameControl
	{
	public:
		FrameControl();
		~FrameControl();

		//
		void SetStyle(const GuiStyle& style);

		//
		// Renders a frame
		void Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& title);

	private:
		Resource<Font> mFont;
		Color mFontColor;
		uint32 mTitleHeight;
		Color mTitleColors[2];
		Color mBodyColors[2];
	};

}
