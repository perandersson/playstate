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
	static const playstate::string FONT_KEY(SAFE_STRING("Button.Font>Font"));

	mFont = style.FindResource<Font>(FONT_KEY);
	
	static const playstate::string BACK_NORMAL_TOP_COLOR(SAFE_STRING("Button.Normal.BackColor.Top>Button.Normal.BackColor>Button.BackColor.Top>Button.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_NORMAL_BOTTOM_COLOR(SAFE_STRING("Button.Normal.BackColor.Bottom>Button.Normal.BackColor>Button.BackColor.Bottom>Button.BackColor>BackColor.Bottom>BackColor"));
	static const playstate::string BACK_HOVER_TOP_COLOR(SAFE_STRING("Button.Hover.BackColor.Top>Button.Hover.BackColor>Button.BackColor.Top>Button.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_HOVER_BOTTOM_COLOR(SAFE_STRING("Button.Hover.BackColor.Bottom>Button.Hover.BackColor>Button.BackColor.Bottom>Button.BackColor>BackColor.Bottom>BackColor"));
	static const playstate::string BACK_DOWN_TOP_COLOR(SAFE_STRING("Button.Down.BackColor.Top>Button.Down.BackColor>Button.BackColor.Top>Button.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_DOWN_BOTTOM_COLOR(SAFE_STRING("Button.Down.BackColor.Bottom>Button.Down.BackColor>Button.BackColor.Bottom>Button.BackColor>BackColor.Bottom>BackColor"));
	
	mButtonColors[0] = style.FindColor(BACK_NORMAL_TOP_COLOR, Color::Black);
	mButtonColors[1] = style.FindColor(BACK_NORMAL_BOTTOM_COLOR, Color::Black);
	mButtonColors[2] = style.FindColor(BACK_HOVER_TOP_COLOR, Color::Black);
	mButtonColors[3] = style.FindColor(BACK_HOVER_BOTTOM_COLOR, Color::Black);
	mButtonColors[4] = style.FindColor(BACK_DOWN_TOP_COLOR, Color::Black);
	mButtonColors[5] = style.FindColor(BACK_DOWN_BOTTOM_COLOR, Color::Black);
	
	static const playstate::string FONT_COLOR_NORMAL(SAFE_STRING("Button.Normal.FontColor>Button.FontColor>FontColor"));
	static const playstate::string FONT_COLOR_HOVER(SAFE_STRING("Button.Hover.FontColor>Button.FontColor>FontColor"));
	static const playstate::string FONT_COLOR_DOWN(SAFE_STRING("Button.Down.FontColor>Button.FontColor>FontColor"));
	
	mFontColors[0] = style.FindColor(FONT_COLOR_NORMAL, Color::Black);
	mFontColors[1] = style.FindColor(FONT_COLOR_HOVER, Color::Black);
	mFontColors[2] = style.FindColor(FONT_COLOR_DOWN, Color::Black);
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
