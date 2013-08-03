#pragma once

#include "../search/result_set.h"
#include "gui_render_block.h"
#include "../search/array_sorter.h"

namespace playstate
{
	class GuiBlockResultSet : public ResultSet<GuiRenderBlock>
	{
	public:
		GuiBlockResultSet();
		virtual ~GuiBlockResultSet();

		//
		// Creates a new render block instance and returns the reference to it 
		GuiRenderBlock* Create(uint32 id);
		
		//
		// Sort this result-set using the supplied array sorter.
		void Sort(IArraySorter<GuiRenderBlock*>* sorter);

		//
		// @return 
		GuiRenderBlock** GetSortedGuiRenderBlocks();

	private:
		GuiRenderBlock** mSortedGuiRenderBlocks;
	};
}
