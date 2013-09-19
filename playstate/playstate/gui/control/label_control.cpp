#include "../../memory/memory.h"
#include "label_control.h"
#include "../canvas.h"
#include "../../math/math.h"
using namespace playstate;

LabelControl::LabelControl()
{
}

LabelControl::~LabelControl()
{
}

void LabelControl::SetStyle(const GuiStyle& style)
{
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFont = style.FindResource<Font>(SAFE_STRING("Label.Font"), mFont);
	mFontColor = style.FindColor(SAFE_STRING("Lable.FontColor"), Color::White);
}

void LabelControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& text)
{
	builder->AddText(mFont.Get(), rect.Position, mFontColor, text);
}
