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
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFont = style.FindResource<Font>(SAFE_STRING("Checkbox.Font"), mFont);
	
	mFontColors[0] = style.FindColor(SAFE_STRING("Checkbox.Unchecked.FontColor"), Color::Black);
	mFontColors[1] = style.FindColor(SAFE_STRING("Checkbox.Checked.FontColor"), Color::Black);
	mCheckboxSize = (uint32)Math::Max(style.FindInt(SAFE_STRING("Checkbox.BoxSize"), 20), 0);

	mCheckboxColors[0] = style.FindColor(SAFE_STRING("Checkbox.Normal.BackColor.Top"), Color::Black);
	mCheckboxColors[1] = style.FindColor(SAFE_STRING("Checkbox.Normal.BackColor.Bottom"), Color::Black);
	mCheckboxColors[2] = style.FindColor(SAFE_STRING("Checkbox.Hover.BackColor.Top"), Color::Black);
	mCheckboxColors[3] = style.FindColor(SAFE_STRING("Checkbox.Hover.BackColor.Bottom"), Color::Black);
	mCheckboxColors[4] = style.FindColor(SAFE_STRING("Checkbox.Down.BackColor.Top"), Color::Black);
	mCheckboxColors[5] = style.FindColor(SAFE_STRING("Checkbox.Down.BackColor.Bottom"), Color::Black);
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
