#include "../memory/memory.h"
#include "render_block_result_set.h"
using namespace playstate;

RenderBlockResultSet::RenderBlockResultSet() 
	: mRenderBlocks(NULL), mSortedRenderBlockIndexes(NULL), mSize(0), mNumElements(0),
	RenderBlocks(mRenderBlocks), SortedIndexes(mSortedRenderBlockIndexes), Size(mSize)
{
	mRenderBlocks = (RenderBlock*)malloc(InitialRenderBlocksCount * sizeof(RenderBlock));
	mSortedRenderBlockIndexes = (uint32*)malloc(InitialRenderBlocksCount * sizeof(uint32));

	mSize = 0;
	mNumElements = InitialRenderBlocksCount;
}

RenderBlockResultSet::~RenderBlockResultSet()
{
	free(mRenderBlocks);
	mRenderBlocks = NULL;
	free(mSortedRenderBlockIndexes);
	mSortedRenderBlockIndexes = NULL;

	mSize = 0;
	mNumElements = 0;
}

RenderBlock& RenderBlockResultSet::CreateAndGet(uint32 id)
{
	if(mSize >= mNumElements) {
		mNumElements += RenderBlocksResizeCount;
		mRenderBlocks = (RenderBlock*)realloc(mRenderBlocks, mNumElements * sizeof(RenderBlock));
		mSortedRenderBlockIndexes = (uint32*)realloc(mSortedRenderBlockIndexes, mNumElements * sizeof(uint32));
	}

	const uint32 index = mSize++;
	mSortedRenderBlockIndexes[index] = id;
	RenderBlock& ref = mRenderBlocks[index];
	ref.Id = id;
	return ref;
}

void RenderBlockResultSet::Reset()
{
	mSize = 0;
}

void RenderBlockResultSet::Sort(IArraySorter<uint32>* sorter)
{
	sorter->Sort(mSortedRenderBlockIndexes, mSize);
}
