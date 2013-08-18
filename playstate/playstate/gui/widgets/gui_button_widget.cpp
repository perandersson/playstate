#include "../../memory/memory.h"
#include "gui_button_widget.h"
using namespace playstate;

GuiButtonWidget::GuiButtonWidget()
	: GuiWidget()
{
}

GuiButtonWidget::GuiButtonWidget(CanvasGroup* group)
	: GuiWidget(group)
{
}

GuiButtonWidget::~GuiButtonWidget()
{
}

void GuiButtonWidget::SetText(const playstate::string& text)
{
	mText = text;
}

const void GuiButtonWidget::BuildWidgetGeometry(GuiGeometryBuilder& builder) const
{
	const uint32 shadowOffset = 3;

	static const Color shadowColor(0, 0, 0, 0.3);

	// Add shadow
	builder.AddQuad(GetAbsolutePosition() - Vector2(shadowOffset, shadowOffset),
		GetSize() + Vector2(shadowOffset * 2, shadowOffset * 2), shadowColor);

	// Add body
	builder.AddGradientQuad(GetAbsolutePosition(), GetSize(), mBackColorTop, mBackColorBottom);

	// Add text?
	//builder.AddText(mFont, GetAbsolutePosition(), Color::White, mText, GetSize());

	GuiWidget::BuildWidgetGeometry(builder);
}

void GuiButtonWidget::OnStyleChanged(const GuiStyle& style)
{
	GuiWidget::OnStyleChanged(style);
}

int playstate::GuiButtonWidget_Factory(lua_State* L)
{
	CanvasGroup* group = luaM_getobject<CanvasGroup>(L);
	if(group != NULL) {
		GuiButtonWidget* widget = new GuiButtonWidget(group);
		luaM_pushobject(L, "GuiButtonWidget", widget);
	} else {
		lua_pushnil(L);
	}

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