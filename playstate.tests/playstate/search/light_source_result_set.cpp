#include <playstate/search/light_source_result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(LightSourceResultSet)
{
	UNIT_TEST(ConstructorIsWorking)
	{
		LightSourceResultSet resultSet;
		
		ASSERT_EQUALS(resultSet.Size, 0);
		ASSERT_NOT_NULL(resultSet.Elements);
	}
	
	UNIT_TEST(CreateBlockFromResultSet)
	{
		LightSourceResultSet resultSet;
		LightSource** block = resultSet.GetOrCreate();

		ASSERT_NOT_NULL(resultSet.Elements);
		ASSERT_EQUALS(&resultSet.Elements[0], block);
	}

	UNIT_TEST(CreateTwoBlocksFromResultSet)
	{
		LightSourceResultSet resultSet;
		LightSource** block1 = resultSet.GetOrCreate();
		LightSource** block2 = resultSet.GetOrCreate();

		ASSERT_NOT_NULL(resultSet.Elements);
		ASSERT_EQUALS(&resultSet.Elements[0], block1);
		ASSERT_EQUALS(&resultSet.Elements[1], block2);
	}
}
