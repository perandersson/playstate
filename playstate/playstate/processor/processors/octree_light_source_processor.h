#pragma once
#include "../light_source_processor_factory.h"
#include "../../scene/tree/octree.h"

namespace playstate
{
	//
	// 
	class OctreeLightSourceProcessor : public ILightSourceProcessor
	{
	public:
		OctreeLightSourceProcessor();
		virtual ~OctreeLightSourceProcessor();
		
	// ILightSourceProcessorFactory
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const;

	private:
		LinkedList<LightSource> mLightSources;
		Octree mOctree;
	};
}
