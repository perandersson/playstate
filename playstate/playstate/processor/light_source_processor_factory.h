#pragma once
#include "../singleton.h"
#include "light_source.h"

namespace playstate
{
	class ILightSourceProcessor
	{
	public:
		virtual ~ILightSourceProcessor() {}

	public:
		//
		// Attaches the supplied light source.
		virtual void AttachLightSource(LightSource* lightSource) = 0;

		//
		// Detaches the supplied light source.
		virtual void DetachLightSource(LightSource* lightSource) = 0;
	};

	//
	// Factory that's responsible for creating stand-alone light source processors.
	class ILightSourceProcessorFactory : public Singleton<ILightSourceProcessorFactory>
	{
	public:
		virtual ~ILightSourceProcessorFactory() {}

	public:
		//
		// @return A light source processor
		virtual ILightSourceProcessor* Create() const = 0;
	};
}
