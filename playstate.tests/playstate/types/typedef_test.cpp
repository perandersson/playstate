#include <playstate/types.h>
#include "../test.h"
using namespace playstate;

TestCase(typedef_VerifySizes, "sizeof tests")
{
	AssertEquals(sizeof(uint16), 2);
	AssertEquals(sizeof(int16), 2);

	AssertEquals(sizeof(uint32), 4);
	AssertEquals(sizeof(int32), 4);
	
	AssertEquals(sizeof(uint64), 8);
	AssertEquals(sizeof(int64), 8);

	AssertEquals(sizeof(float32), 4);
	AssertEquals(sizeof(float64), 8);
}
