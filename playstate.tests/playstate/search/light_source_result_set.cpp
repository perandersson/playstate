#include <playstate/search/light_source_result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(LightSourceResultSet)
{
	UNIT_TEST(ConstructorIsWorking)
	{
		LightSourceResultSet resultSet;
		
		ASSERT_EQUALS(resultSet.GetNumLightSources(), 0);
		ASSERT_NOT_NULL(resultSet.GetLightSources());
	}
	
	UNIT_TEST(AddLightSourceToResultSet)
	{
		LightSourceResultSet unitToTest;
		LightSource lightSource;
		
		unitToTest.AddResult(&lightSource);
		
		ASSERT_EQUALS(unitToTest.GetNumLightSources(), 1);
		ASSERT_NOT_NULL(unitToTest.GetLightSources());
		ASSERT_EQUALS(unitToTest.GetLightSources()[0], &lightSource);
	}
}
