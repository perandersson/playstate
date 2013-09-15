#pragma once
#include "../linked_list.h"
#include "../scene/spatial_node.h"
#include "../rendering/render_state.h"
#include "../search/render_block_result_set.h"

namespace playstate
{
	class IRenderProcessor;

	class IRenderable
	{
	public:
		virtual ~IRenderable() {}

	public:
		//
		// Performs the pre-render stage. The purpos of this stage is to prepare the render blocks
		// before rendering. The collected render blocks will then be sent to the render pipeline.
		//
		// @param state
		// @param builder
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet) = 0;
	};

	class Renderable : public IRenderable, public SpatialNode
	{
	public:
		LinkedListLink<Renderable> RenderableLink;

	public:
		//
		// Constructor
		Renderable();

		//
		// Destructor
		virtual ~Renderable();

		//
		// Attach this renderable instance so that it can be displayed on the screen
		// @param processor
		void Attach(IRenderProcessor* processor);

		//
		// Detaches this renderable instance from the screen.
		void Detach();

		//
		// Hides this renderable item.
		void Hide();

		//
		// Shows this renderable item in the scene.
		// @remark This method can only be called on an attached renderable object.
		void Show();

	private:
		IRenderProcessor* mAttachedToProcessor;
		bool mVisible;
	};
}
