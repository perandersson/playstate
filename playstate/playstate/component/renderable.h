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
		
		//
		// Shows this renderable item in the scene.
		// @remark This method can only be called on an attached renderable object.
		virtual void Show() = 0;
		
		//
		// Hides this renderable item from the scene.
		virtual void Hide() = 0;
	};
}
