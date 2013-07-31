#include <playstate/search/render_block_result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(RenderBlockResultSet)
{
	UNIT_TEST(ConstructorIsWorking)
	{
		RenderBlockResultSet resultSet;
		
		ASSERT_EQUALS(resultSet.GetSize(), 0);
		ASSERT_NOT_NULL(resultSet.GetElements());
	}

	UNIT_TEST(CreateBlockWithUniqueId)
	{
		const uint32 id = 100;

		RenderBlockResultSet resultSet;
		const RenderBlock* block = resultSet.Create(id);
		
		ASSERT_EQUALS(resultSet.GetSize(), 1);
		ASSERT_NOT_NULL(resultSet.GetElements());
		ASSERT_EQUALS(&resultSet.GetElements()[0], block);

		ASSERT_EQUALS(block->Id, id);
	}

	UNIT_TEST(CreateTwoBlocksWithUniqueId)
	{
		const uint32 id1 = 100;
		const uint32 id2 = 201;

		RenderBlockResultSet resultSet;
		const RenderBlock* block1 = resultSet.Create(id1);
		const RenderBlock* block2 = resultSet.Create(id2);
		
		ASSERT_EQUALS(resultSet.GetSize(), 2);
		ASSERT_NOT_NULL(resultSet.GetElements());
		ASSERT_EQUALS(&resultSet.GetElements()[0], block1);
		ASSERT_EQUALS(&resultSet.GetElements()[1], block2);

		ASSERT_EQUALS(block1->Id, id1);
		ASSERT_EQUALS(block2->Id, id2);
	}
}
