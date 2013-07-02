#include "unit_test.h"
#include "test_suite.h"
using namespace playstate;

UnitTest::UnitTest(const char* name, const char* file, const long line)
	: Name(name), File(file), Line(line)
{
}

UnitTest::~UnitTest()
{
}
