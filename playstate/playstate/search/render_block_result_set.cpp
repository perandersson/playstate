#include "../memory/memory.h"
#include "render_block_result_set.h"
using namespace playstate;

RenderBlockResultSet::RenderBlockResultSet() 
	: mRenderBlocks(NULL), mSortedRenderBlocks(NULL), mSize(0), mNumElements(0),
	RenderBlocks(mRenderBlocks), SortedRenderBlocks(mSortedRenderBlocks), Size(mSize)
{
	mRenderBlocks = (RenderBlock*)malloc(InitialRenderBlocksCount * sizeof(RenderBlock));
	mSortedRenderBlocks = (RenderBlock**)malloc(InitialRenderBlocksCount * sizeof(RenderBlock*));
	
	mSize = 0;
	mNumElements = InitialRenderBlocksCount;
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mRenderBlocks);
	mRenderBlocks = NULL;
	
	mSize = 0;
	mNumElements = 0;
}

RenderBlock& RenderBlockResultSet::CreateAndGet(uint32 id)
{
	if(mSize >= mNumElements) {
		mNumElements += RenderBlocksResizeCount;
		mRenderBlocks = (RenderBlock*)realloc(mRenderBlocks, mNumElements * sizeof(RenderBlock));
		mSortedRenderBlocks = (RenderBlock**)realloc(mSortedRenderBlocks, mNumElements * sizeof(RenderBlock*));
	}

	const uint32 index = mSize++;
	RenderBlock* ref = &mRenderBlocks[index];
	mSortedRenderBlocks[index] = ref;
	memset(ref, 0, sizeof(RenderBlock));
	ref->Id = id;
	return *ref;
}

void RenderBlockResultSet::Reset()
{
	mSize = 0;
}

void RenderBlockResultSet::Sort(IArraySorter<RenderBlock*>* sorter)
{
	sorter->Sort(mSortedRenderBlocks, mSize);
}
