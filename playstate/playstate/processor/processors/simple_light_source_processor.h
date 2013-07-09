#pragma once
#include "../light_source_processor_factory.h"

namespace playstate
{
	//
	// 
	class SimpleLightSourceProcessor : public ILightSourceProcessor
	{
	public:
		SimpleLightSourceProcessor();
		virtual ~SimpleLightSourceProcessor();
		
	// ILightSourceProcessorFactory
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);

	private:
		LinkedList<LightSource, &LightSource::LightSourceLink> mLightSources;
	};

	class SimpleLightSourceProcessorFactory : public ILightSourceProcessorFactory
	{
	public:
		SimpleLightSourceProcessorFactory();
		virtual ~SimpleLightSourceProcessorFactory();

	public:
		virtual ILightSourceProcessor* Create() const;
	};
}
