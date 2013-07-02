#include "test_suite.h"
#include "test_callback.h"
#include "unit_test.h"
#include "test_exception.h"
using namespace playstate;

TestSuite::TestSuite(const std::string& name)
	: mName(name),
	Name(mName)
{
}

TestSuite::~TestSuite()
{
}

void TestSuite::RunUnitTests(ITestCallback* callback)
{
	callback->BeginTestSuite(this);

	UnitTests::iterator it = mUnitTests.begin();
	UnitTests::iterator end = mUnitTests.end();
	for(;it != end; ++it) {
		UnitTest* test = *it;
		callback->BeginUnitTest(this, test);
		try {
			test->Run(this);
		} catch(TestException e) {
			callback->TestFailure(this, test, e.File.c_str(), e.Message.c_str(), e.Line);
		} catch(glmock::IValidationException e) {
			for(unsigned int i = 0; i < e.Count; ++i) {
				char tmp[512];
				sprintf(tmp, "'%s' when calling '%s'", e.Errors[i].Error, e.Errors[i].Command);
				callback->TestFailure(this, test, test->File.c_str(), tmp, test->Line);
			}
		}
		callback->EndUnitTest(this, test);
	}

	callback->EndTestSuite(this);
}

void TestSuite::AddUnitTest(UnitTest* test)
{
	mUnitTests.push_back(test);
}
