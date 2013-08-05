#include "../memory/memory.h"
#include "gui_render_processor.h"
using namespace playstate;

GuiRenderProcessor::GuiRenderProcessor()
{
}

GuiRenderProcessor::~GuiRenderProcessor()
{
}

void GuiRenderProcessor::AttachRenderable(Renderable* renderable)
{
}

void GuiRenderProcessor::DetachRenderable(Renderable* renderable)
{
}

bool GuiRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	return false;
}
