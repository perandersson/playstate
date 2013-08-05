#include "../../memory/memory.h"
#include "gui_frame_component.h"
#include "../../rendering/vertex_buffer.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
#include "../../rendering/render_system.h"
#include "../gui_geometry_builder.h"

using namespace playstate;

GuiFrameComponent::GuiFrameComponent(const std::string& title, uint32 width, uint32 height)
	: Component(), Renderable(), Scriptable(), mVertexBuffer(NULL), mTitle(title), mWidth(width), mHeight(height)
{
}

GuiFrameComponent::~GuiFrameComponent()
{
	if(mVertexBuffer != NULL) {
		delete mVertexBuffer;
		mVertexBuffer = NULL;
	}
}

void GuiFrameComponent::OnComponentAdded()
{
	// 1. Create quad based on width and height
	// 2. Create vertices block for title text if neccessary.

	const uint32 ShadowColorIndex = 0;
	const uint32 BackgroundColorIndex = 1;
	const uint32 FrontfaceColorIndex = 1;

	const uint32 ShadowOffset = 3;
	const uint32 TitleSize = 10;

	GuiGeometryBuilder ggb(RenderSystem::Get());
	ggb.AddQuad(Rect(0, 0, mWidth + ShadowOffset*2, mHeight + ShadowOffset*2), ShadowColorIndex);
	ggb.AddQuad(Rect(ShadowOffset, ShadowOffset, mWidth, mHeight), BackgroundColorIndex);
	ggb.AddQuad(Rect(ShadowOffset, ShadowOffset + TitleSize, mWidth, mHeight - TitleSize), FrontfaceColorIndex);
	mVertexBuffer = ggb.Build();

	Renderable::Attach(GetNode()->GetGroup());
}

void GuiFrameComponent::OnComponentRemoved()
{
	Renderable::Detach();
}

void GuiFrameComponent::SetSize(uint32 width, uint32 height)
{
	if(mWidth != width || mHeight != height) {
		mWidth = width;
		mHeight = height;

		// Rebuild the vertex buffer
	}
}

void GuiFrameComponent::SetTitle(const std::string& title)
{
	mTitle = title;
}

void GuiFrameComponent::Collect(const RenderState& state, RenderBlockResultSet* resultSet)
{
	if(BIT_ISSET(state.Filter, RenderStateFilter::USER_INTERFACE)) {
		RenderBlock* block = resultSet->Create(100);
		block->VertexBuffer = mVertexBuffer;
		block->ModelMatrix = GetNode()->GetModelMatrix();
	}

	//resultSet->Create(
}

namespace playstate
{
	int GuiFrameComponent_Factory(lua_State* L)
	{
		uint32 height = lua_tointeger(L, -1); 
		uint32 width = lua_tointeger(L, -2);
		std::string title = lua_tostring(L, -3);
		lua_pop(L, 3);

		GuiFrameComponent* component = new GuiFrameComponent(title, width, height);
		luaM_pushobject(L, "GuiFrameComponent", component);
		return 1;
	}
}
