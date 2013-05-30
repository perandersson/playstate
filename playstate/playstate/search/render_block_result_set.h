#pragma once

#include "../rendering/render_block.h"
#include "array_sorter.h"

namespace playstate
{
	// 
	// 
	static const uint32 InitialRenderBlocksCount = 1000;
	static const uint32 RenderBlocksResizeCount = 100;
	
	//
	// 
	class RenderBlockResultSet
	{
	public:
		RenderBlockResultSet();
		~RenderBlockResultSet();

		//
		// Creates a new render block instance and returns the reference to it 
		RenderBlock& CreateAndGet(uint32 id);

		//
		// Resets the render blocks result-set.
		// @remark This does not release the memory allocated for the RenderBlocks array. All memory
		//	is reused until. You have to destroy the RenderBlockResultSet object to delete the allocated memory.
		void Reset();

		//
		// Sort this result-set using the supplied array sorter
		void Sort(IArraySorter<RenderBlock*>* sorter);

	public:
		// Read-only property for the blocks
		RenderBlock*& const RenderBlocks;

		// Read-only property for the sorted render blocks
		RenderBlock**& const SortedRenderBlocks;

		// Read-only property for the number of blocks
		const uint32& Size;

	private:
		RenderBlock* mRenderBlocks;
		RenderBlock** mSortedRenderBlocks;
		uint32 mSize;
		uint32 mNumElements;
	};
}
