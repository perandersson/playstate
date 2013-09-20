#include "../../memory/memory.h"
#include "checkbox_control.h"
#include "../canvas.h"
#include "../../math/math.h"
using namespace playstate;

CheckboxControl::CheckboxControl()
{
}

CheckboxControl::~CheckboxControl()
{
}

void CheckboxControl::SetStyle(const GuiStyle& style)
{
	static const playstate::string FONT_KEY(SAFE_STRING("Checkbox.Font>Font"));

	mFont = style.FindResource<Font>(FONT_KEY);

	static const playstate::string FONT_COLOR_UNCHECKED(SAFE_STRING("Checkbox.Unchecked.FontColor>Checkbox.FontColor>FontColor"));
	static const playstate::string FONT_COLOR_CHECKED(SAFE_STRING("Checkbox.Checked.FontColor>Checkbox.FontColor>FontColor"));
	
	mFontColors[0] = style.FindColor(FONT_COLOR_UNCHECKED, Color::Black);
	mFontColors[1] = style.FindColor(FONT_COLOR_CHECKED, Color::Black);

	static const playstate::string CHECKBOX_SIZE(SAFE_STRING("Checkbox.BoxSize"));

	mCheckboxSize = (uint32)Math::Max(style.FindInt(CHECKBOX_SIZE, 20), 0);

	static const playstate::string BACK_NORMAL_TOP_COLOR(
		SAFE_STRING("Checkbox.Normal.BackColor.Top>Checkbox.Normal.BackColor>Checkbox.BackColor.Top>Checkbox.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_NORMAL_BOTTOM_COLOR(
		SAFE_STRING("Checkbox.Normal.BackColor.Bottom>Checkbox.Normal.BackColor>Checkbox.BackColor.Bottom>Checkbox.BackColor>BackColor.Bottom>BackColor"));
	static const playstate::string BACK_HOVER_TOP_COLOR(
		SAFE_STRING("Checkbox.Hover.BackColor.Top>Checkbox.Hover.BackColor>Checkbox.BackColor.Top>Checkbox.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_HOVER_BOTTOM_COLOR(
		SAFE_STRING("Checkbox.Hover.BackColor.Bottom>Checkbox.Hover.BackColor>Checkbox.BackColor.Bottom>Checkbox.BackColor>BackColor.Bottom>BackColor"));
	static const playstate::string BACK_DOWN_TOP_COLOR(
		SAFE_STRING("Checkbox.Down.BackColor.Top>Checkbox.Down.BackColor>Checkbox.BackColor.Top>Checkbox.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BACK_DOWN_BOTTOM_COLOR(
		SAFE_STRING("Checkbox.Down.BackColor.Bottom>Checkbox.Down.BackColor>Checkbox.BackColor.Bottom>Checkbox.BackColor>BackColor.Bottom>BackColor"));

	mCheckboxColors[0] = style.FindColor(BACK_NORMAL_TOP_COLOR, Color::Black);
	mCheckboxColors[1] = style.FindColor(BACK_NORMAL_BOTTOM_COLOR, Color::Black);
	mCheckboxColors[2] = style.FindColor(BACK_HOVER_TOP_COLOR, Color::Black);
	mCheckboxColors[3] = style.FindColor(BACK_HOVER_BOTTOM_COLOR, Color::Black);
	mCheckboxColors[4] = style.FindColor(BACK_DOWN_TOP_COLOR, Color::Black);
	mCheckboxColors[5] = style.FindColor(BACK_DOWN_BOTTOM_COLOR, Color::Black);
}

bool CheckboxControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, bool toggled, const playstate::string& text)
{
	// Status used 
	GuiMouseState::Enum state = canvas.GetMouseState(rect);

	const uint32 checkboxBorderOffset = 1U;
	const uint32 checkboxToggleOffset = checkboxBorderOffset + 3U;
	
	// Add border around checkbox button
	const Rect checkboxButtonBorder(rect.Position, Size(mCheckboxSize, mCheckboxSize));
	builder->AddQuad(checkboxButtonBorder, Color::Black);

	// Add checkbox button
	const Color& boxColorTop = mCheckboxColors[2 * (uint32)state];
	const Color& boxColorBottom = mCheckboxColors[2 * (uint32)state + 1];
	const Rect checkboxButton(rect.Position + Point(checkboxBorderOffset, checkboxBorderOffset), 
		Size(mCheckboxSize - (checkboxBorderOffset * 2), mCheckboxSize - (checkboxBorderOffset * 2)));
	builder->AddGradientQuad(checkboxButton, boxColorTop, boxColorBottom);

	// Add geometry if checkbox is toggled (enabled)
	if(toggled) {
		const Rect toggledCoords(rect.Position + Point(checkboxToggleOffset, checkboxToggleOffset), 
			Size(mCheckboxSize - (checkboxToggleOffset * 2), mCheckboxSize - (checkboxToggleOffset * 2)));
		builder->AddQuad(toggledCoords, Color::White);
	}

	// Add checkbox text if one is present.
	if(text.length() > 0 && !mFont.IsNull()) {
		const Color& fontColor = mFontColors[toggled ? 1 : 0];
		builder->AddText(mFont.Get(), rect + Rect(mCheckboxSize + 10, 0, 0, 0), fontColor, text, FontAlign::LEFT);
	}

	if(state == GuiMouseState::HOVER && canvas.GetMouseClick() == MouseButtons::LEFT) {
		toggled = !toggled;
	}

	return toggled;
}
