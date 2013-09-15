#pragma once

#include "result_set.h"
#include "../memory/memory_pool.h"
#include "../processor/light_source.h"

namespace playstate
{

	//
	// 
	class LightSourceResultSet : public IResultSet<LightSource>
	{
	public:
		LightSourceResultSet();
		~LightSourceResultSet();

		//
		//
		void AddResult(LightSource* lightSource);

	// IResultSet<T>
	public:
		virtual uint32 GetNumResults() const;
		virtual IResultSet<LightSource>::Type GetResultData() const;
		virtual void Reset();

	private:
		MemoryPool<LightSource*> mMemoryPool;
	};
}
