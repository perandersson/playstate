#include <playstate/types.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(Typedefs)
{
	UNIT_TEST(typedef_VerifySizes)
	{
		ASSERT_EQUALS(sizeof(uint16), 2);
		ASSERT_EQUALS(sizeof(int16), 2);

		ASSERT_EQUALS(sizeof(uint32), 4);
		ASSERT_EQUALS(sizeof(int32), 4);
	
		ASSERT_EQUALS(sizeof(uint64), 8);
		ASSERT_EQUALS(sizeof(int64), 8);

		ASSERT_EQUALS(sizeof(float32), 4);
		ASSERT_EQUALS(sizeof(float64), 8);
	}
}