#include "../memory/memory.h"
#include "light_source_result_set.h"
using namespace playstate;

LightSourceResultSet::LightSourceResultSet() 
	: mMemoryPool(20, 5)
{
}

LightSourceResultSet::~LightSourceResultSet()
{
}

void LightSourceResultSet::AddResult(LightSource* lightSource)
{
	LightSource** ptr = mMemoryPool.Allocate();
	*ptr = lightSource;
}

uint32 LightSourceResultSet::GetNumResults() const
{
	return mMemoryPool.GetSize();
}

IResultSet<LightSource>::Type LightSourceResultSet::GetResultData() const
{
	return mMemoryPool.GetFirstElement();
}

void LightSourceResultSet::Reset()
{
	mMemoryPool.Reset();
}