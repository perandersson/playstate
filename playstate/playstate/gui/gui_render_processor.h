#pragma once

#include "../processor/render_processor_factory.h"

namespace playstate
{
	class GuiRenderProcessor : public IRenderProcessor
	{
	public:
		GuiRenderProcessor();
		virtual ~GuiRenderProcessor();

	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;
	};
}
