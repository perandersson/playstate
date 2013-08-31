#include "../../memory/memory.h"
#include "gui_button_widget.h"
using namespace playstate;

GuiButtonWidget::GuiButtonWidget()
	: GuiWidget()
{
}

GuiButtonWidget::~GuiButtonWidget()
{
}

void GuiButtonWidget::SetText(const playstate::string& text)
{
	mText = text;
}

void GuiButtonWidget::PreRender(GuiGeometryBuilder& builder)
{
	const uint32 shadowOffset = 3;

	static const Color shadowColor(0, 0, 0, 0.3);

	// Add shadow
	builder.AddQuad(GetAbsolutePosition() - Vector2(shadowOffset, shadowOffset),
		GetSize() + Vector2(shadowOffset * 2, shadowOffset * 2), shadowColor);

	// Add body
	builder.AddGradientQuad(GetAbsolutePosition(), GetSize(), mBackColorTop, mBackColorBottom);

	if(!mFont.IsNull()) {
		builder.AddText(mFont.Get(), GetAbsolutePosition(), mFrontColor, mText, GetSize());
	}

	GuiWidget::PreRender(builder);
}

void GuiButtonWidget::OnStyleChanged(const GuiStyle& style)
{
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFrontColor = style.FindColor(SAFE_STRING("FrontColor"), Color::White);
	GuiWidget::OnStyleChanged(style);
}

int playstate::GuiButtonWidget_Factory(lua_State* L)
{
	GuiButtonWidget* widget = new GuiButtonWidget();
	luaM_pushobject(L, "GuiButtonWidget", widget);
	return 1;
}

int playstate::GuiButtonWidget_SetText(lua_State* L)
{
	playstate::string text = lua_tostring(L, -1);
	lua_pop(L, 1);

	GuiButtonWidget* widget = luaM_popobject<GuiButtonWidget>(L);
	if(widget != NULL) {
		widget->SetText(text);
	}

	return 0;
}