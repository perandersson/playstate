#pragma once
#include "../render_processor.h"
#include "../../linked_list.h"

namespace playstate
{
	//
	// A render processor where all the renderable items are put in a first -> last linked list.
	// Useful for those SceneGroups with very few nodes.
	class LinkedListRenderProcessor : public IRenderProcessor
	{
	public:
		LinkedListRenderProcessor();
		virtual ~LinkedListRenderProcessor();
				
	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	private:
		LinkedList<Renderable> mRenderables;
	};
}
