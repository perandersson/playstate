#pragma once
#include "../array_sorter.h"

namespace playstate
{
	//
	// A quick-sort implementation used by the engine to sort id item numbers.
	// Useful for making sore we are rendering the same items in succession instead of drawing
	// them on the screen randomly (random => many state changes => slow)
	class QuickSortArraySorter : public IArraySorter<uint32>
	{
	public:
		QuickSortArraySorter();
		virtual ~QuickSortArraySorter();

	// IArraySorter
	public:
		virtual void Sort(uint32* items, uint32 numItems);

	private:
		void QuickSort(uint32* items, int32 left, int32 right);
	};
}
