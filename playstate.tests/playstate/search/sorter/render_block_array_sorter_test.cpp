#include <playstate/search/sorter/render_block_array_sorter.h>
#include "../../test.h"
using namespace playstate;

TestCase(RenderBlockArraySorter_ThreeElements, "")
{
	const uint32 numItems = 3;
	RenderBlock* b1 = new RenderBlock(); b1->Id = 3;
	RenderBlock* b2 = new RenderBlock(); b2->Id = 100;
	RenderBlock* b3 = new RenderBlock(); b3->Id = 2;
	RenderBlock* blocks[numItems] = { b1, b2, b3 };
	RenderBlock* expected[numItems] = { b3, b1, b2 };

	RenderBlockArraySorter sorter;
	sorter.Sort(blocks, numItems);

	AssertEquals(blocks[0], expected[0]);
	AssertEquals(blocks[1], expected[1]);
	AssertEquals(blocks[2], expected[2]);
}