#include "test_exception.h"
#include "unit_test.h"
using namespace playstate;

TestException::TestException(UnitTest* test, const char* file, const char* message, long line)
	: Test(test), File(file), Message(message), Line(line)
{}

TestException::~TestException()
{}
