#include <playstate/search/sorter/quick_sort_array_sorter.h>
#include "../../test.h"
using namespace playstate;

TestCase(QuickSortArraySorterEmptyResultSet, "")
{
	uint32 arr[1] = {0};
	QuickSortArraySorter sorter;
	sorter.Sort(arr, 0);
}

TestCase(QuickSortArraySorterOneResultSet, "")
{
	const uint32 arraySize = 1;
	uint32 arr[arraySize] = {10};
	QuickSortArraySorter sorter;
	sorter.Sort(arr, arraySize);

	AssertEquals(arr[0], 10);
}

TestCase(QuickSortArraySorterTwoResultSet, "")
{
	const uint32 arraySize = 2;
	uint32 arr[arraySize] = {10, 3};
	QuickSortArraySorter sorter;
	sorter.Sort(arr, arraySize);

	AssertEquals(arr[0], 3);
	AssertEquals(arr[1], 10);
}

TestCase(QuickSortArraySorterThreeResultSet, "")
{
	const uint32 arraySize = 3;
	uint32 arr[arraySize] = {10, 3, 2};
	QuickSortArraySorter sorter;
	sorter.Sort(arr, arraySize);

	AssertEquals(arr[0], 2);
	AssertEquals(arr[1], 3);
	AssertEquals(arr[2], 10);
}

TestCase(QuickSortArraySorterTenResultSet, "")
{
	const uint32 arraySize = 10;
	uint32 arr[arraySize] = {10, 3, 2, 2, 10, 4, 10, 100, 1000, 10000};
	const uint32 expected[arraySize] = {2, 2, 3, 4, 10, 10, 10, 100, 1000, 10000};
	QuickSortArraySorter sorter;
	sorter.Sort(arr, arraySize);

	AssertEquals(arr[0], expected[0]);
	AssertEquals(arr[1], expected[1]);
	AssertEquals(arr[2], expected[2]);
	AssertEquals(arr[3], expected[3]);
	AssertEquals(arr[4], expected[4]);
	AssertEquals(arr[5], expected[5]);
	AssertEquals(arr[6], expected[6]);
	AssertEquals(arr[7], expected[7]);
	AssertEquals(arr[8], expected[8]);
	AssertEquals(arr[9], expected[9]);
}
