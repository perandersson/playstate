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
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFont = style.FindResource<Font>(SAFE_STRING("Slider.Font"), mFont);

	mSliderBackColors[0] = style.FindColor(SAFE_STRING("Slider.Normal.BackColor.Top"), Color::Black);
	mSliderBackColors[1] = style.FindColor(SAFE_STRING("Slider.Normal.BackColor.Bottom"), Color::Black);
	mSliderFontColor[0] = style.FindColor(SAFE_STRING("Slider.Normal.FontColor"), Color::White);
	mSliderBarColors[0] = style.FindColor(SAFE_STRING("Slider.Normal.BarColor.Top"), Color::Black);
	mSliderBarColors[1] = style.FindColor(SAFE_STRING("Slider.Normal.BarColor.Bottom"), Color::Black);

	mSliderBackColors[2] = style.FindColor(SAFE_STRING("Slider.Hover.BackColor.Top"), Color::Black);
	mSliderBackColors[3] = style.FindColor(SAFE_STRING("Slider.Hover.BackColor.Bottom"), Color::Black);
	mSliderFontColor[1] = style.FindColor(SAFE_STRING("Slider.Hover.FontColor"), Color::White);
	mSliderBarColors[2] = style.FindColor(SAFE_STRING("Slider.Hover.BarColor.Top"), Color::Black);
	mSliderBarColors[3] = style.FindColor(SAFE_STRING("Slider.Hover.BarColor.Bottom"), Color::Black);

	mSliderBackColors[4] = style.FindColor(SAFE_STRING("Slider.Down.BackColor.Top"), Color::Black);
	mSliderBackColors[5] = style.FindColor(SAFE_STRING("Slider.Down.BackColor.Bottom"), Color::Black);
	mSliderFontColor[2] = style.FindColor(SAFE_STRING("Slider.Down.FontColor"), Color::White);
	mSliderBarColors[4] = style.FindColor(SAFE_STRING("Slider.Down.BarColor.Top"), Color::Black);
	mSliderBarColors[5] = style.FindColor(SAFE_STRING("Slider.Down.BarColor.Bottom"), Color::Black);
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
