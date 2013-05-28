#include "../../memory/memory.h"
#include "quick_sort_array_sorter.h"
using namespace playstate;

QuickSortArraySorter::QuickSortArraySorter()
{
}

QuickSortArraySorter::~QuickSortArraySorter()
{
}

void QuickSortArraySorter::Sort(uint32* items, uint32 numItems)
{
	assert_not_null(items);

	if(numItems <= 1)
		return;

	if(numItems == 2) {
		uint32 tmp1 = items[0];
		if(tmp1 > items[1]) {
			items[0] = items[1];
			items[1] = tmp1;
		}
		return;
	}
	
	QuickSort(items, 0, numItems - 1);
}

void QuickSortArraySorter::QuickSort(uint32* items, int32 left, int32 right)
{
	int32 i = left, j = right;
    uint32 tmp;
	uint32 pivot = items[(left + right) / 2];
 
    while (i <= j) {
        while (items[i] < pivot)
                i++;
        while (items[j] > pivot)
                j--;
        if (i <= j) {
                tmp = items[i];
                items[i] = items[j];
                items[j] = tmp;
                i++;
                j--;
        }
    };
 
    if (left < j)
        QuickSort(items, left, j);
    if (i < right)
        QuickSort(items, i, right);
}
