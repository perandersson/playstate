#include <playstate/memory/memory_pool.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(MemoryPool)
{
	struct MockData
	{
		int value;
	};

	UNIT_TEST(EmptyMemoryPool)
	{
		const uint32 initialSize = 10;
		const uint32 resizeSize = 5;

		MemoryPool<MockData> unitToTest(initialSize, resizeSize);
		
		ASSERT_EQUALS(unitToTest.GetSize(), 0);
		ASSERT_EQUALS(unitToTest.GetMemorySize(), initialSize * sizeof(MockData));
		ASSERT_FALSE(unitToTest.IsResizeRequired());
	}

	UNIT_TEST(TwoElements)
	{
		const uint32 initialSize = 10;
		const uint32 resizeSize = 5;

		MemoryPool<MockData> unitToTest(initialSize, resizeSize);

		unitToTest.Allocate()->value = 1;
		unitToTest.Allocate()->value = 2;
		
		ASSERT_EQUALS(unitToTest.GetSize(), 2);
		ASSERT_EQUALS(unitToTest.GetMemorySize(), initialSize * sizeof(MockData));

		MockData* firstElement = unitToTest.GetFirstElement();
		ASSERT_NOT_NULL(firstElement);

		ASSERT_EQUALS(firstElement[0].value, 1);
		ASSERT_EQUALS(firstElement[1].value, 2);
	}

	UNIT_TEST(AboutToBeResized)
	{
		const uint32 initialSize = 3;
		const uint32 resizeSize = 4;

		MemoryPool<MockData> unitToTest(initialSize, resizeSize);
		
		unitToTest.Allocate();
		ASSERT_FALSE(unitToTest.IsResizeRequired());

		unitToTest.Allocate();
		ASSERT_FALSE(unitToTest.IsResizeRequired());

		unitToTest.Allocate();
		ASSERT_TRUE(unitToTest.IsResizeRequired());
	}
	
	UNIT_TEST(ResizedMemoryPool)
	{
		const uint32 initialSize = 3;
		const uint32 resizeSize = 4;

		MemoryPool<MockData> unitToTest(initialSize, resizeSize);
		
		unitToTest.Allocate();
		unitToTest.Allocate();
		unitToTest.Allocate();
		unitToTest.Allocate();

		ASSERT_FALSE(unitToTest.IsResizeRequired());
		ASSERT_EQUALS(unitToTest.GetSize(), 4);
		ASSERT_EQUALS(unitToTest.GetMemorySize(), (initialSize + resizeSize) * sizeof(MockData));
	}
}
