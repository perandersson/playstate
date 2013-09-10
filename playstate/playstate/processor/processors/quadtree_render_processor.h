#pragma once
#include "../render_processor.h"
#include "../../scene/tree/quadtree_node.h"

namespace playstate
{
	//
	// Container for handling renderable objects
	class QuadTreeRenderProcessor : public IRenderProcessor
	{
	public:
		QuadTreeRenderProcessor();
		virtual ~QuadTreeRenderProcessor();
				
	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	private:
		LinkedList<Renderable> mRenderables;
		QuadTreeNode mQuadTree;
	};
}
