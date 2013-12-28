#pragma once
#include "../singleton.h"
#include "../search/find_query.h"
#include "../search/render_block_result_set.h"

namespace playstate
{
	struct RenderBlock;
	class SceneNode;

	//
	// The processor used to contain all renderable objects and then query them using a specific find query.
	class IRenderProcessor
	{
	public:
		virtual ~IRenderProcessor() {}

	public:
		//
		// Attaches a new renderable instance so that it can receive render events.
		// @param renderable
		virtual void AttachRenderable(SceneNode* node) = 0;

		//
		// Detaches the supplied renderable instancer so that it no longer receives any render events.
		// @param renderable
		virtual void DetachRenderable(SceneNode* node) = 0;

		//
		// Find renderable items in this scene based on the supplied camera and put the results
		// inside the target vector
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const = 0;
	};
}
