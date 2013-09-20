#include "../../memory/memory.h"
#include "button_control.h"
#include "../canvas.h"
#include "../../math/math.h"
using namespace playstate;

ButtonControl::ButtonControl()
{
}

ButtonControl::~ButtonControl()
{
}

void ButtonControl::SetStyle(const GuiStyle& style)
{
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFont = style.FindResource<Font>(SAFE_STRING("Button.Font"), mFont);

	mButtonColors[0] = style.FindColor(SAFE_STRING("Button.Normal.BackColor.Top"), Color::Black);
	mButtonColors[1] = style.FindColor(SAFE_STRING("Button.Normal.BackColor.Bottom"), Color::Black);
	mButtonColors[2] = style.FindColor(SAFE_STRING("Button.Hover.BackColor.Top"), Color::Black);
	mButtonColors[3] = style.FindColor(SAFE_STRING("Button.Hover.BackColor.Bottom"), Color::Black);
	mButtonColors[4] = style.FindColor(SAFE_STRING("Button.Down.BackColor.Top"), Color::Black);
	mButtonColors[5] = style.FindColor(SAFE_STRING("Button.Down.BackColor.Bottom"), Color::Black);
	
	mFontColors[0] = style.FindColor(SAFE_STRING("Button.Normal.FontColor"), Color::Black);
	mFontColors[1] = style.FindColor(SAFE_STRING("Button.Hover.FontColor"), Color::Black);
	mFontColors[2] = style.FindColor(SAFE_STRING("Button.Down.FontColor"), Color::Black);
}

bool ButtonControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& text)
{
	// Status used 
	GuiMouseState::Enum state = canvas.GetMouseState(rect);

	const Color& buttonColorTop = mButtonColors[2 * (uint32)state];
	const Color& buttonColorBotton = mButtonColors[2 * (uint32)state + 1];
	const Color& fontColor = mFontColors[(uint32)state];

	// Add body
	builder->AddGradientQuad(rect, buttonColorTop, buttonColorBotton);

	if(mFont.IsNotNull() && !text.empty()) {
		builder->AddText(mFont.Get(), rect, fontColor, text, FontAlign::CENTER);
	}

	if(state == GuiMouseState::HOVER && canvas.GetMouseClick() == MouseButtons::LEFT) {
		return true;
	}
	return false;
}
