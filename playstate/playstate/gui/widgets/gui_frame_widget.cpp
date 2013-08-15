#include "../../memory/memory.h"
#include "gui_frame_widget.h"
using namespace playstate;

GuiFrameWidget::GuiFrameWidget(const std::string& title, float32 width, float32 height)
	: GuiWidget(), mTitle(title)
{
	mSize.Set(width, height);
}

GuiFrameWidget::GuiFrameWidget(CanvasGroup* group, const std::string& title, float32 width, float32 height)
	: GuiWidget(group), mTitle(title)
{
	mSize.Set(width, height);
}

GuiFrameWidget::~GuiFrameWidget()
{
}

const void GuiFrameWidget::BuildWidgetGeometry(GuiGeometryBuilder& builder) const
{
	const uint32 shadowOffset = 3;
	const uint32 shadowPaletteIndex = 0;

	const uint32 titleHeight = 15;
	const uint32 titleTopPaletteIndex = 1;
	const uint32 titleBottomPaletteIndex = 2;

	const uint32 bodyTopPaletteIndex = 3;
	const uint32 bodyBottomPaletteIndex = 4;

	// Add shadow
	builder.AddQuad(GetPosition() - Vector2(shadowOffset, shadowOffset),
		GetSize() + Vector2(shadowOffset * 2, shadowOffset * 2), shadowPaletteIndex);

	// Add title
	builder.AddGradientQuad(GetPosition(), Vector2(GetSize().X, titleHeight), titleTopPaletteIndex, titleTopPaletteIndex,
		titleBottomPaletteIndex, titleBottomPaletteIndex);

	// Add body
	builder.AddGradientQuad(GetPosition() + Vector2(0.0f, titleHeight), GetSize() - Vector2(0.0f, titleHeight), bodyTopPaletteIndex, bodyTopPaletteIndex,
		bodyBottomPaletteIndex, bodyBottomPaletteIndex);

	// Add text?

	// Add child controls
	//for
}

int playstate::GuiFrameWidget_Factory(lua_State* L)
{
	float32 height = lua_tonumber(L, -1);
	float32 width = lua_tonumber(L, -2);
	std::string title = lua_tostring(L, -3);
	lua_pop(L, 3);

	CanvasGroup* group = luaM_getobject<CanvasGroup>(L);
	if(group != NULL) {
		GuiFrameWidget* widget = new GuiFrameWidget(group, title, width, height);
		luaM_pushobject(L, "GuiFrameWidget", widget);
	} else {
		lua_pushnil(L);
	}

	return 1;
}
