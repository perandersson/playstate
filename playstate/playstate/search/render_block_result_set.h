#pragma once

#include "../rendering/render_block.h"
#include "array_sorter.h"
#include "result_set.h"

namespace playstate
{
	// 
	// 
	static const uint32 InitialRenderBlocksCount = 1000;
	static const uint32 RenderBlocksResizeCount = 100;
	
	//
	// 
	class RenderBlockResultSet : public ResultSet<RenderBlock>
	{
	public:
		RenderBlockResultSet();
		virtual ~RenderBlockResultSet();

		//
		// Creates a new render block instance and returns the reference to it 
		RenderBlock* Create(uint32 id);
		
		//
		// Sort this result-set using the supplied array sorter.
		void Sort(IArraySorter<RenderBlock*>* sorter);

		//
		// @return 
		RenderBlock** GetSortedRenderBlocks();

	private:
		RenderBlock** mSortedRenderBlocks;
	};
}
