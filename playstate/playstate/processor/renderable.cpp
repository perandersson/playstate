#include "../memory/memory.h"
#include "renderable.h"
#include "render_processor_factory.h"
using namespace playstate;

Renderable::Renderable() : OctreeNode()
{
}

Renderable::~Renderable()
{
}

void Renderable::Attach(IRenderProcessor* processor)
{
	processor->AttachRenderable(this);
}

void Renderable::Detach()
{
	RenderableLink.Unlink();
	OctreeLink.Unlink();
}
