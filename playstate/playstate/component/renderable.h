#pragma once
#include "../linked_list.h"
#include "../rendering/render_state.h"
#include "../search/render_block_result_set.h"

namespace playstate
{
	class IRenderProcessor;
	class ISpatialTree;
	
	//
	// 
	class IRenderable
	{
	public:
		virtual ~IRenderable() {}

	public:
		//
		// Performs the pre-render stage. The purpos of this stage is to prepare the current instances render blocks
		// before rendering. The collected render blocks will then be sent to the render pipeline.
		//
		// @param state
		// @param builder
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet) = 0;
	};

	//
	//
	class Renderable : public IRenderable
	{
	public:
		LinkedListLink<Renderable> RenderableNodeLink;

	public:
		Renderable();
		virtual ~Renderable();
	};
}
