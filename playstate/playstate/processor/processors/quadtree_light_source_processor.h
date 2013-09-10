#pragma once
#include "../light_source_processor.h"
#include "../../scene/tree/quadtree_node.h"

namespace playstate
{
	//
	// 
	class QuadTreeLightSourceProcessor : public ILightSourceProcessor
	{
	public:
		QuadTreeLightSourceProcessor();
		virtual ~QuadTreeLightSourceProcessor();
		
	// ILightSourceProcessorFactory
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const;

	private:
		LinkedList<LightSource> mLightSources;
		QuadTreeNode mQuadTree;
	};
}
