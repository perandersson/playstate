#pragma once
#include "../array_sorter.h"
#include "../../rendering/render_block.h"

namespace playstate
{
	//
	// A quick-sort implementation used by the engine to sort render blocks.
	// Useful for making sore we are rendering the same items in succession instead of drawing
	// them on the screen randomly (random => many state changes => slow)
	class RenderBlockArraySorter : public IArraySorter<RenderBlock*>
	{
	public:
		RenderBlockArraySorter();
		virtual ~RenderBlockArraySorter();

	// IArraySorter
	public:
		virtual void Sort(RenderBlock** items, uint32 numItems);

	private:
		void QuickSort(RenderBlock** items, int32 left, int32 right);
	};
}
