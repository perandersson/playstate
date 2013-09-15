#include "../memory/memory.h"
#include "render_block_result_set.h"
using namespace playstate;

RenderBlockResultSet::RenderBlockResultSet() 
	: mMemoryPool(InitialRenderBlocksCount, RenderBlocksResizeCount), mSortedRenderBlocks(NULL), mNumElements(0), mCurrentMemoryBlock(0)
{
	mCurrentMemoryBlock = mMemoryPool.GetFirstElement();
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mSortedRenderBlocks);
	mSortedRenderBlocks = NULL;
}

RenderBlock* RenderBlockResultSet::Create(uint32 id)
{
	RenderBlock* block = mMemoryPool.Allocate();
	block->Id = id;
	return block;
}

bool RenderBlockResultSet::IsResizeRequired() const
{
	const uint32 numBlocksToDraw = mMemoryPool.GetMemorySize() / sizeof(RenderBlock);
	return numBlocksToDraw > mNumElements;
}

void RenderBlockResultSet::Sort(IArraySorter<RenderBlock*>* sorter)
{
	if(IsResizeRequired()) {
		const uint32 size = mMemoryPool.GetSize();
		mSortedRenderBlocks = (RenderBlock**)realloc(mSortedRenderBlocks, size * sizeof(RenderBlock**));
		mNumElements = size;
		mCurrentMemoryBlock = NULL;
	}

	if(mMemoryPool.GetFirstElement() != mCurrentMemoryBlock) {
		RenderBlock* data = mMemoryPool.GetFirstElement();
		for(uint32 i = 0; i < mNumElements; ++i) {
			mSortedRenderBlocks[i] = &data[i];
		}
		mCurrentMemoryBlock = data;
	}
	if(mSortedRenderBlocks != NULL)
		sorter->Sort(mSortedRenderBlocks, mMemoryPool.GetSize());
}

IResultSet<RenderBlock>::Type RenderBlockResultSet::GetResultData() const
{
	return mSortedRenderBlocks;
}

void RenderBlockResultSet::Reset()
{
	mMemoryPool.Reset();
}

uint32 RenderBlockResultSet::GetNumResults() const
{
	return mMemoryPool.GetSize();
}

