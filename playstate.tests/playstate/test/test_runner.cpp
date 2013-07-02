#include "test_runner.h"
#include "test_callback.h"
#include "test_suite.h"
#include "unit_test.h"
#include <iostream>
#include <sstream>
using namespace playstate;

TestRunner::TestRunner()
{
}

TestRunner::~TestRunner()
{
}

TestRunner& TestRunner::Get()
{
	static TestRunner obj;
	return obj;
}

namespace {
	class ConsoleOutputCallback : public ITestCallback
	{
	public:
		ConsoleOutputCallback() {
		}

		virtual ~ConsoleOutputCallback() {
		}

	// ITestCallback
	public:
		virtual void Begin()
		{
			std::cout << std::endl << "STARTS RUNNING TESTS" << std::endl;
			std::cout << std::endl;
		}

		virtual void End()
		{
		}

		virtual void BeginTestSuite(const TestSuite* suite)
		{
			std::cout << "[" << suite->Name << "]" << std::endl;
		}
		
		virtual void EndTestSuite(const TestSuite* suite)
		{
			std::cout << std::endl;
		}
		
		virtual void BeginUnitTest(const TestSuite* suite, const UnitTest* test)
		{
			std::cout << "\tRunning: " << test->Name;
		}

		virtual void EndUnitTest(const TestSuite* suite, const UnitTest* test)
		{
			if(mFailures.empty()) {
				std::cout << " (OK)" << std::endl;
			} else {
				std::cout << " (FAILED) " << std::endl;
				for(size_t i = 0; i < mFailures.size(); ++i) {
					std::cout << mFailures[i] << std::endl;
				}
			}
			mFailures.clear();
		}

		virtual void TestFailure(const TestSuite* suite, const UnitTest* test, const char* file, const char* message, long line)
		{
			std::stringstream ss;
			ss << "\t" << message << ":" << std::endl << "\t" << file << "(" << line << ")";
			mFailures.push_back(ss.str());
		}

	private:
		std::vector<std::string> mFailures;
	};

	class WrappedOutputCallback : public ITestCallback
	{
	public:
		WrappedOutputCallback(ITestCallback* callback)
			: mCallback(callback), mError(false),
			Error(mError)
		{
		}

		~WrappedOutputCallback()
		{
		}

	public:
		virtual void Begin()
		{
			mCallback->Begin();
		}

		virtual void End()
		{
			mCallback->End();
		}

		virtual void BeginTestSuite(const TestSuite* suite)
		{
			mCallback->BeginTestSuite(suite);
		}
		
		virtual void EndTestSuite(const TestSuite* suite)
		{
			mCallback->EndTestSuite(suite);
		}

		virtual void BeginUnitTest(const TestSuite* suite, const UnitTest* test)
		{
			mCallback->BeginUnitTest(suite, test);
		}

		virtual void EndUnitTest(const TestSuite* suite, const UnitTest* test)
		{
			mCallback->EndUnitTest(suite, test);
		}

		virtual void TestFailure(const TestSuite* suite, const UnitTest* test, const char* file, const char* message, long line)
		{
			mCallback->TestFailure(suite, test, file, message, line);
			mError = true;
		}

	public:
		bool& const Error;

	private:
		ITestCallback* mCallback;
		bool mError;
	};
}

int TestRunner::Run()
{
	ConsoleOutputCallback callback;
	return Run(&callback);
}

int TestRunner::Run(ITestCallback* callback)
{
	WrappedOutputCallback wrapped(callback);
	wrapped.Begin();

	TestSuites& suites = Get().mSuites;
	TestSuites::iterator it = suites.begin();
	TestSuites::iterator end = suites.end();
	for(;it != end; ++it) {
		TestSuite* suite = it->second;
		suite->RunUnitTests(&wrapped);
	}

	return wrapped.Error ? -1 : 0;
}

void TestRunner::AddUnitTest(const char* suiteName, UnitTest* unitTest)
{
	TestSuites& suites = Get().mSuites;

	std::string name(suiteName);
	TestSuites::iterator i = suites.find(name);
	TestSuite* suite = NULL;
	if(i != suites.end()) {
		suite = i->second;
	} else {
		suite = new TestSuite(suiteName);
		suites.insert(std::make_pair(name, suite));
	}
	suite->AddUnitTest(unitTest);
}
