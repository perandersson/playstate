#include "../memory/memory.h"
#include "render_block_result_set.h"
using namespace playstate;

RenderBlockResultSet::RenderBlockResultSet() 
	: mMemoryPool(InitialRenderBlocksCount, RenderBlocksResizeCount)
{
	mSortedRenderBlocks = (RenderBlock**)malloc(InitialRenderBlocksCount * sizeof(RenderBlock**));
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mSortedRenderBlocks);
	mSortedRenderBlocks = NULL;
}

RenderBlock* RenderBlockResultSet::Create(uint32 id)
{
	if(mMemoryPool.IsResizeRequired()) {
		mSortedRenderBlocks = (RenderBlock**)realloc(mSortedRenderBlocks, mMemoryPool.GetMemorySize() + (RenderBlocksResizeCount * sizeof(RenderBlock**)));
	}

	RenderBlock* block = mMemoryPool.Allocate();
	mSortedRenderBlocks[mMemoryPool.GetSize() - 1] = block;
	block->Id = id;
	return block;
}

void RenderBlockResultSet::Sort(IArraySorter<RenderBlock*>* sorter)
{
	sorter->Sort(mSortedRenderBlocks, mMemoryPool.GetSize());
}

RenderBlock** RenderBlockResultSet::GetRenderBlocks()
{
	return mSortedRenderBlocks;
}

void RenderBlockResultSet::Reset()
{
	mMemoryPool.Reset();
}

uint32 RenderBlockResultSet::GetSize() const
{
	return mMemoryPool.GetSize();
}

