#pragma once
#include "../render_processor_factory.h"
#include "../../scene/tree/octree.h"

namespace playstate
{
	//
	// Container for handling renderable objects
	class OctreeRenderProcessor : public IRenderProcessor
	{
	public:
		// 
		// Constructor
		OctreeRenderProcessor();

		//
		// Destructor
		virtual ~OctreeRenderProcessor();
				
	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	private:
		LinkedList<Renderable> mRenderables;
		Octree mOctree;
	};

	//
	// Factory for creating the linked list update processor
	class OctreeRenderProcessorFactory : public IRenderProcessorFactory
	{
	public:
		OctreeRenderProcessorFactory();
		virtual ~OctreeRenderProcessorFactory();

	// IRenderProcessorFactory
	public:
		virtual IRenderProcessor* Create() const;
	};
}
