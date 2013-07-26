#pragma once
#include "../singleton.h"
#include "light_source.h"
#include "../search/light_source_result_set.h"
#include "../search/find_query.h"

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

		//
		// Method used to query light sources.
		// 
		// @param query
		// @param target
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const = 0;
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
