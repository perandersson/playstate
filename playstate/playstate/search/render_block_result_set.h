#pragma once

#include "../rendering/render_block.h"
#include "array_sorter.h"
#include "result_set.h"
#include "../memory/memory_pool.h"

namespace playstate
{
	// 
	// 
	static const uint32 InitialRenderBlocksCount = 1000;
	static const uint32 RenderBlocksResizeCount = 100;
	
	//
	// Specific result-set for render blocks.
	class RenderBlockResultSet : public IResultSet<RenderBlock*>
	{
	public:
		RenderBlockResultSet();
		virtual ~RenderBlockResultSet();

		//
		// Creates a new render block instance and returns the reference to it 
		RenderBlock* Create(uint32 id);
		
		//
		// @return 
		RenderBlock** GetRenderBlocks();

		//
		// 
		void Reset();

		//
		// @return The size of this result set
		uint32 GetSize() const;

		//
		// Sorts this resultset using the supplied array sorter
		//
		// @param sorter
		void Sort(IArraySorter<RenderBlock*>* sorter);

	private:
		bool IsResizeRequired() const;

	private:
		MemoryPool<RenderBlock> mMemoryPool;
		const RenderBlock* mCurrentMemoryBlock;
		RenderBlock** mSortedRenderBlocks;
		uint32 mNumElements;
	};
}
