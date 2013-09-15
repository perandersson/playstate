#include <playstate/search/light_source_result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(LightSourceResultSet)
{
	UNIT_TEST(ConstructorIsWorking)
	{
		LightSourceResultSet resultSet;
		
		ASSERT_EQUALS(resultSet.GetNumResults(), 0);
		ASSERT_NOT_NULL(resultSet.GetResultData());
	}
	
	UNIT_TEST(AddLightSourceToResultSet)
	{
		LightSourceResultSet unitToTest;
		LightSource lightSource;
		
		unitToTest.AddResult(&lightSource);
		
		ASSERT_EQUALS(unitToTest.GetNumResults(), 1);
		ASSERT_NOT_NULL(unitToTest.GetResultData());
		ASSERT_EQUALS(unitToTest.GetResultData()[0], &lightSource);
	}
}
