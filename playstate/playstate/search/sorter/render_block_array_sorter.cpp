#include "../../memory/memory.h"
#include "render_block_array_sorter.h"
using namespace playstate;

RenderBlockArraySorter::RenderBlockArraySorter()
{
}

RenderBlockArraySorter::~RenderBlockArraySorter()
{
}

void RenderBlockArraySorter::Sort(RenderBlock** items, uint32 numItems)
{
	assert_not_null(items);

	if(numItems <= 2)
		return;

	QuickSort(items, 0, numItems - 1);
}

void RenderBlockArraySorter::QuickSort(RenderBlock** items, int32 left, int32 right)
{
	int32 i = left, j = right;
    RenderBlock* tmp;
	uint32 pivot = items[(left + right) / 2]->Id;
 
    while (i <= j) {
        while (items[i]->Id < pivot)
                i++;
        while (items[j]->Id > pivot)
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
