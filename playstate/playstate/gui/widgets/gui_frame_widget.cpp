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
	const uint32 titlePaletteIndex = 1;

	const uint32 bodyPaletteIndex = 2;

	// Add shadow
	builder.AddQuad(GetPosition() - Vector2(shadowOffset, shadowOffset),
		GetSize() + Vector2(shadowOffset * 2, shadowOffset * 2), shadowPaletteIndex);

	// Add title
	builder.AddQuad(GetPosition(), Vector2(0.0f, titleHeight), titlePaletteIndex);

	// Add body
	builder.AddQuad(GetPosition() + Vector2(0.0f, titleHeight), GetSize() - Vector2(0.0f, titleHeight), bodyPaletteIndex);

	// Add text?
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
