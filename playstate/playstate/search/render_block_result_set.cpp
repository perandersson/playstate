#include "../memory/memory.h"
#include "render_block_result_set.h"
using namespace playstate;

RenderBlockResultSet::RenderBlockResultSet() 
	: ResultSet<RenderBlock>(InitialRenderBlocksCount, RenderBlocksResizeCount), SortedRenderBlocks(mSortedRenderBlocks)
{
	mSortedRenderBlocks = (RenderBlock**)malloc(InitialRenderBlocksCount * sizeof(RenderBlock*));
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mSortedRenderBlocks);
	mSortedRenderBlocks = NULL;
}

RenderBlock* RenderBlockResultSet::Create(uint32 id)
{
	RenderBlock* block = GetOrCreate();
	mSortedRenderBlocks[mSize - 1] = block;
	block->Id = id;
	return block;
}

void RenderBlockResultSet::Sort(IArraySorter<RenderBlock*>* sorter)
{
	sorter->Sort(mSortedRenderBlocks, mSize);
}
