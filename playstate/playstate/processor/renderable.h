#pragma once
#include "../linked_list.h"
#include "../scene/tree/octree_node.h"
#include "../rendering/render_state.h"
#include "../search/render_block_result_set.h"

namespace playstate
{
	class IRenderProcessor;

	class Renderable : public OctreeNode
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
		// Collects all the building blocks for this object. Then it's the pipelines responsibility
		// to draw those items onto the screen.
		//
		// @param builder
		// @param state
		virtual void CollectBuildingBlocks(RenderBlockResultSet& resultSet, RenderState& state) = 0;
	};
}
