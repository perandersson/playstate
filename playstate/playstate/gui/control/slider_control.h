#pragma once
#include "../gui_style.h"
#include "../gui_geometry_builder.h"
#include "../gui_mouse_state.h"

namespace playstate
{
	class Canvas;

	class SliderControl
	{
	public:
		SliderControl();
		~SliderControl();

		//
		void SetStyle(const GuiStyle& style);

		//
		// Renders a slider based on the supplied properties
		float32 Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, float32 value, float32 leftValue, float32 rightValue,
			float32 stepValue);

	private:
		Resource<Font> mFont;

		Color mSliderBackColors[2 * GuiMouseState::NUM_STATES];
		Color mSliderFontColor[3];
		Color mSliderBarColors[2 * GuiMouseState::NUM_STATES];
	};

}
