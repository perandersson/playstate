#include "../../memory/memory.h"
#include "gui_frame_widget.h"
using namespace playstate;

GuiFrameWidget::GuiFrameWidget()
	: GuiWidget()
{
}

GuiFrameWidget::~GuiFrameWidget()
{
}

void GuiFrameWidget::SetTitle(const playstate::string& title)
{
	mTitle = title;
}

void GuiFrameWidget::PreRender(GuiGeometryBuilder& builder)
{
	const uint32 shadowOffset = 3;
	const uint32 titleHeight = 25;

	static const Color shadowColor(0, 0, 0, 0.3);

	static const Color titleTop = Color::HexToRGB("#EEEEEE");
	static const Color titleBottom = Color::HexToRGB("#777777");
	
	// Add shadow
	builder.AddQuad(GetAbsolutePosition() - Vector2(shadowOffset, shadowOffset),
		GetSize() + Vector2(shadowOffset * 2, shadowOffset * 2), shadowColor);

	// Add title
	builder.AddGradientQuad(GetAbsolutePosition(), Vector2(GetSize().X, titleHeight), titleTop, titleBottom);

	// Add body
	builder.AddGradientQuad(GetAbsolutePosition() + Vector2(0.0f, titleHeight), GetSize() - Vector2(0.0f, titleHeight), mBackColorTop, mBackColorBottom);

	if(!mFont.IsNull()) {
		builder.AddText(mFont.Get(), GetAbsolutePosition(), mFrontColor, mTitle, Vector2(GetSize().X, titleHeight));
	}
	
	
	GuiWidget::PreRender(builder);
}

void GuiFrameWidget::OnStyleChanged(const GuiStyle& style)
{
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFrontColor = style.FindColor(SAFE_STRING("FrontColor"), Color::White);
	GuiWidget::OnStyleChanged(style);
}

int playstate::GuiFrameWidget_Factory(lua_State* L)
{
	GuiFrameWidget* widget = new GuiFrameWidget();
	luaM_pushobject(L, "GuiFrameWidget", widget);
	return 1;
}

int playstate::GuiFrameWidget_SetTitle(lua_State* L)
{
	playstate::string title = lua_tostring(L, -1);
	lua_pop(L, 1);

	GuiFrameWidget* widget = luaM_popobject<GuiFrameWidget>(L);
	if(widget != NULL) {
		widget->SetTitle(title);
	}

	return 0;
}