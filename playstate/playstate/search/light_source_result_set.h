#pragma once

#include "result_set.h"
#include "../memory/memory_pool.h"
#include "../processor/light_source.h"

namespace playstate
{

	//
	// 
	class LightSourceResultSet : public IResultSet<LightSource*>
	{
	public:
		LightSourceResultSet();
		~LightSourceResultSet();

		//
		//
		void AddResult(LightSource* lightSource);

		//
		// @return
		uint32 GetNumLightSources() const;

		//
		// @return 
		LightSource** GetLightSources();

		void Reset();

	private:
		MemoryPool<LightSource*> mMemoryPool;
	};
}
