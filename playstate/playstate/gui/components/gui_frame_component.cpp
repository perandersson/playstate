#include "../../memory/memory.h"
#include "gui_frame_component.h"
#include "../../rendering/vertex_buffer.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
using namespace playstate;

GuiFrameComponent::GuiFrameComponent(const std::string& title, uint32 width, uint32 height)
	: Component(), Renderable(), mVertexBuffer(NULL), mTitle(title), mWidth(width), mHeight(height)
{
}

GuiFrameComponent::~GuiFrameComponent()
{
	delete mVertexBuffer;
	mVertexBuffer = NULL;
}

void GuiFrameComponent::OnComponentAdded()
{
	//mVertexBuffer = 

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
	//resultSet->Create(
}
