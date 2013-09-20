#include "../../memory/memory.h"
#include "slider_control.h"
#include "../canvas.h"
#include "../../math/math.h"
using namespace playstate;

SliderControl::SliderControl()
{
}

SliderControl::~SliderControl()
{
}

void SliderControl::SetStyle(const GuiStyle& style)
{
	static const playstate::string FONT_KEY(SAFE_STRING("Frame.Font>Font"));

	mFont = style.FindResource<Font>(FONT_KEY);

	static const playstate::string NORMAL_FONT_COLOR(SAFE_STRING("Slider.Normal.FontColor>Slider.FontColor>FontColor"));
	static const playstate::string HOVER_FONT_COLOR(SAFE_STRING("Slider.Hover.FontColor>Slider.FontColor>FontColor"));
	static const playstate::string DOWN_FONT_COLOR(SAFE_STRING("Slider.Down.FontColor>Slider.FontColor>FontColor"));

	mSliderFontColor[0] = style.FindColor(NORMAL_FONT_COLOR, Color::White);
	mSliderFontColor[1] = style.FindColor(HOVER_FONT_COLOR, Color::White);
	mSliderFontColor[2] = style.FindColor(DOWN_FONT_COLOR, Color::White);

	static const playstate::string NORMAL_BACK_TOP_COLOR(SAFE_STRING("Slider.Normal.BackColor.Top>Slider.Normal.BackColor>Slider.BackColor.Top>Slider.BackColor>BackColor"));
	static const playstate::string NORMAL_BACK_BOTTOM_COLOR(SAFE_STRING("Slider.Normal.BackColor.Bottom>Slider.Normal.BackColor>Slider.BackColor.Bottom>Slider.BackColor>BackColor"));
	static const playstate::string HOVER_BACK_TOP_COLOR(SAFE_STRING("Slider.Hover.BackColor.Top>Slider.Hover.BackColor>Slider.BackColor.Top>Slider.BackColor>BackColor"));
	static const playstate::string HOVER_BACK_BOTTOM_COLOR(SAFE_STRING("Slider.Hover.BackColor.Bottom>Slider.Hover.BackColor>Slider.BackColor.Bottom>Slider.BackColor>BackColor"));
	static const playstate::string DOWN_BACK_TOP_COLOR(SAFE_STRING("Slider.Down.BackColor.Top>Slider.Down.BackColor>Slider.BackColor.Top>Slider.BackColor>BackColor"));
	static const playstate::string DOWN_BACK_BOTTOM_COLOR(SAFE_STRING("Slider.Down.BackColor.Bottom>Slider.Down.BackColor>Slider.BackColor.Bottom>Slider.BackColor>BackColor"));
	
	mSliderBackColors[0] = style.FindColor(NORMAL_BACK_TOP_COLOR, Color::Black);
	mSliderBackColors[1] = style.FindColor(NORMAL_BACK_BOTTOM_COLOR, Color::Black);
	mSliderBackColors[2] = style.FindColor(HOVER_BACK_TOP_COLOR, Color::Black);
	mSliderBackColors[3] = style.FindColor(HOVER_BACK_BOTTOM_COLOR, Color::Black);
	mSliderBackColors[4] = style.FindColor(DOWN_BACK_TOP_COLOR, Color::Black);
	mSliderBackColors[5] = style.FindColor(DOWN_BACK_BOTTOM_COLOR, Color::Black);

	static const playstate::string NORMAL_BAR_TOP_COLOR(SAFE_STRING("Slider.Normal.BarColor.Top>Slider.Normal.BarColor>Slider.BarColor.Top>Slider.BarColor"));
	static const playstate::string NORMAL_BAR_BOTTOM_COLOR(SAFE_STRING("Slider.Normal.BarColor.Bottom>Slider.Normal.BarColor>Slider.BarColor.Bottom>Slider.BarColor"));
	static const playstate::string HOVER_BAR_TOP_COLOR(SAFE_STRING("Slider.Hover.BarColor.Top>Slider.Hover.BarColor>Slider.BarColor.Top>Slider.BarColor"));
	static const playstate::string HOVER_BAR_BOTTOM_COLOR(SAFE_STRING("Slider.Hover.BarColor.Bottom>Slider.Hover.BarColor>Slider.BarColor.Bottom>Slider.BarColor"));
	static const playstate::string DOWN_BAR_TOP_COLOR(SAFE_STRING("Slider.Down.BarColor.Top>Slider.Down.BarColor>Slider.BarColor.Top>Slider.BarColor"));
	static const playstate::string DOWN_BAR_BOTTOM_COLOR(SAFE_STRING("Slider.Down.BarColor.Bottom>Slider.Down.BarColor>Slider.BarColor.Bottom>Slider.BarColor"));

	mSliderBarColors[0] = style.FindColor(NORMAL_BAR_TOP_COLOR, Color::Black);
	mSliderBarColors[1] = style.FindColor(NORMAL_BAR_BOTTOM_COLOR, Color::Black);
	mSliderBarColors[2] = style.FindColor(HOVER_BAR_TOP_COLOR, Color::Black);
	mSliderBarColors[3] = style.FindColor(HOVER_BAR_BOTTOM_COLOR, Color::Black);
	mSliderBarColors[4] = style.FindColor(DOWN_BAR_TOP_COLOR, Color::Black);
	mSliderBarColors[5] = style.FindColor(DOWN_BAR_BOTTOM_COLOR, Color::Black);
}

float32 SliderControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, float32 value, float32 leftValue, float32 rightValue, float32 stepValue)
{
	// Status used 
	GuiMouseState::Enum state = canvas.GetMouseState(rect);

	const Color& backColorTop = mSliderBackColors[2 * (uint32)state];
	const Color& backColorBottom = mSliderBackColors[2 * (uint32)state + 1];
	const Color& fontColor = mSliderFontColor[(uint32)state];
	const Color& barColorTop = mSliderBarColors[2 * (uint32)state];
	const Color& barColorBottom = mSliderBarColors[2 * (uint32)state + 1];

	// Add body
	builder->AddGradientQuad(rect, backColorTop, backColorBottom);

	// Add slider value
	const float valueDt = (leftValue + (rightValue - leftValue));

	const float32 valueOffset = rightValue - (rightValue - leftValue);
	const float32 dt = (value - valueOffset) / (rightValue - valueOffset);
	const Rect filledValueCoords(rect.Position, Size(rect.Width * dt, rect.Height));
	builder->AddGradientQuad(filledValueCoords, barColorTop, barColorBottom);
	
	if(state == GuiMouseState::DOWN) {
		const Point mousePos = canvas.GetMousePosition();

		// Calculate value based on the mouse coordinates in relation to this control
		const float32 pixelsFromControlWidth = mousePos.X - rect.X;
		const float32 mouseDt = pixelsFromControlWidth / (float32)rect.Width;

		// Lerp between leftValue and rightValue
		value = Math::Lerp(leftValue, rightValue, mouseDt);
	}

	char tmp[50];
	sprintf(tmp, "%.2f", value);
	
	builder->AddText(mFont.Get(), rect, fontColor, tmp, FontAlign::CENTER);

	return value;
}
