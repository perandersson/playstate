#include "../memory/memory.h"
#include "gui_block_result_set.h"
using namespace playstate;

GuiBlockResultSet::GuiBlockResultSet()
	: ResultSet<GuiRenderBlock>(50, 20), mSortedGuiRenderBlocks(NULL)
{
}

GuiBlockResultSet::~GuiBlockResultSet()
{
}

GuiRenderBlock* GuiBlockResultSet::Create(uint32 id)
{
	if(mSize >= mNumElements) {
		mSortedGuiRenderBlocks = (GuiRenderBlock**)realloc(mSortedGuiRenderBlocks, (mNumElements + mBlocksResizeCount) * sizeof(GuiRenderBlock**));
	}
	
	GuiRenderBlock* block = ResultSet<GuiRenderBlock>::GetOrCreate();
	mSortedGuiRenderBlocks[mSize - 1] = block;
	block->Id = id;
	return block;
}
		
void GuiBlockResultSet::Sort(IArraySorter<GuiRenderBlock*>* sorter)
{
	sorter->Sort(mSortedGuiRenderBlocks, mSize);
}

GuiRenderBlock** GuiBlockResultSet::GetSortedGuiRenderBlocks()
{
	return mSortedGuiRenderBlocks;
}
