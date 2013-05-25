#include "../memory/memory.h"
#include "render_block_builder.h"

#include <cassert>

#define PTR_TO_INT(ptr) ((sp_uint32)ptr)

using namespace playstate;

const uint32 MaxNumVisibleItemsOnScreen = 10000;

namespace {
	RenderBlock _blocks[MaxNumVisibleItemsOnScreen + 1];
	RenderBlock* _blocksPtr[MaxNumVisibleItemsOnScreen + 1];
}

RenderBlockBuilder::RenderBlockBuilder() : mBlocks(NULL), mNumBlocks(0)
{
	// The maximum number of renderable blocks on the scene at the same time
	// TODO Make this more dynamic?
	mBlocks = _blocks;
	memset(mBlocks, 0, sizeof(RenderBlock) * (MaxNumVisibleItemsOnScreen + 1));
	memset(_blocksPtr, 0, sizeof(RenderBlock*) * (MaxNumVisibleItemsOnScreen + 1));
}

RenderBlockBuilder::~RenderBlockBuilder()
{
}

bool RenderBlockBuilder::SortAndSave(FindResultSet<RenderBlock>* target)
{
	if(mNumBlocks == 0)
		return false;

	target->SetMemory(_blocksPtr);
	_blocksPtr[mNumBlocks] = 0;

	if(mNumBlocks <= 2)
		return true;

	QuickSort(_blocksPtr, 0, mNumBlocks - 1);
	return true;
}

RenderBlock* RenderBlockBuilder::NewBlock(uint32 id)
{
	assert(mNumBlocks <= MaxNumVisibleItemsOnScreen && "There are to many visible items on the screen at the same time!");

	RenderBlock* block = &mBlocks[mNumBlocks];
	_blocksPtr[mNumBlocks] = block;
	mNumBlocks++;
	block->Id = id;
	return block;
}

void RenderBlockBuilder::Clean()
{
	mNumBlocks = 0;
}

void RenderBlockBuilder::QuickSort(RenderBlock** arr, int32 left, int32 right)
{
	  int32 i = left, j = right;
      RenderBlock* tmp;
	  uint32 pivot = arr[(left + right) / 2]->Id;
 
      while (i <= j) {
            while (arr[i]->Id < pivot)
                  i++;
            while (arr[j]->Id > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };
 
      if (left < j)
            QuickSort(arr, left, j);
      if (i < right)
            QuickSort(arr, i, right);
}
