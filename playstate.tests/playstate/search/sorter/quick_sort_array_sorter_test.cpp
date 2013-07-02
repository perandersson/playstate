#include <playstate/search/sorter/quick_sort_array_sorter.h>
#include "../../test/test.h"
using namespace playstate;

TEST_SUITE(QuickSortArraySorter)
{
	UNIT_TEST(QuickSortArraySorterEmptyResultSet)
	{
		uint32 arr[1] = {0};
		QuickSortArraySorter sorter;
		sorter.Sort(arr, 0);
	}

	UNIT_TEST(QuickSortArraySorterOneResultSet)
	{
		const uint32 arraySize = 1;
		uint32 arr[arraySize] = {10};
		QuickSortArraySorter sorter;
		sorter.Sort(arr, arraySize);

		ASSERT_EQUALS(arr[0], 10);
	}

	UNIT_TEST(QuickSortArraySorterTwoResultSet)
	{
		const uint32 arraySize = 2;
		uint32 arr[arraySize] = {10, 3};
		QuickSortArraySorter sorter;
		sorter.Sort(arr, arraySize);

		ASSERT_EQUALS(arr[0], 3);
		ASSERT_EQUALS(arr[1], 10);
	}

	UNIT_TEST(QuickSortArraySorterThreeResultSet)
	{
		const uint32 arraySize = 3;
		uint32 arr[arraySize] = {10, 3, 2};
		QuickSortArraySorter sorter;
		sorter.Sort(arr, arraySize);

		ASSERT_EQUALS(arr[0], 2);
		ASSERT_EQUALS(arr[1], 3);
		ASSERT_EQUALS(arr[2], 10);
	}

	UNIT_TEST(QuickSortArraySorterTenResultSet)
	{
		const uint32 arraySize = 10;
		uint32 arr[arraySize] = {10, 3, 2, 2, 10, 4, 10, 100, 1000, 10000};
		const uint32 expected[arraySize] = {2, 2, 3, 4, 10, 10, 10, 100, 1000, 10000};
		QuickSortArraySorter sorter;
		sorter.Sort(arr, arraySize);

		ASSERT_EQUALS(arr[0], expected[0]);
		ASSERT_EQUALS(arr[1], expected[1]);
		ASSERT_EQUALS(arr[2], expected[2]);
		ASSERT_EQUALS(arr[3], expected[3]);
		ASSERT_EQUALS(arr[4], expected[4]);
		ASSERT_EQUALS(arr[5], expected[5]);
		ASSERT_EQUALS(arr[6], expected[6]);
		ASSERT_EQUALS(arr[7], expected[7]);
		ASSERT_EQUALS(arr[8], expected[8]);
		ASSERT_EQUALS(arr[9], expected[9]);
	}
}
