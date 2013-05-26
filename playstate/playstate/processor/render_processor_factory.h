#pragma once
#include "../singleton.h"
#include "renderable.h"
#include "../search/find_query.h"
#include "../search/find_result_set.h"

namespace playstate
{
	struct RenderBlock;

	class IRenderProcessor
	{
	public:
		virtual ~IRenderProcessor() {}

	public:
		//
		// Attaches a new renderable instance so that it can receive render events.
		// @param renderable
		virtual void AttachRenderable(Renderable* renderable) = 0;

		//
		// Detaches the supplied renderable instancer so that it no longer receives any render events.
		// @param renderable
		virtual void DetachRenderable(Renderable* renderable) = 0;

		//
		// Find renderable items in this scene based on the supplied camera and put the results
		// inside the target vector
		virtual bool Find(const FindQuery& query, FindResultSet<RenderBlock>* target) const = 0;
	};

	//
	// Factory that's responsible for creating stand-alone render processors
	class IRenderProcessorFactory : public Singleton<IRenderProcessorFactory>
	{
	public:
		virtual ~IRenderProcessorFactory() {}

	public:
		//
		// @return A render processor
		virtual IRenderProcessor* Create() const = 0;
	};
}
