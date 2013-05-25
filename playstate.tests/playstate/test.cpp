#include "test.h"
#include <list>
#include <iostream>
using namespace playstate;

namespace {
	TestCaseRunner* _runner = NULL;
}

TestCaseRunner::TestCaseRunner(const char* name, const char* description, const char* file, const long line)
	: Name(name), Description(description), File(file), Line(line)
{
	TestApplication::AddTestCase(this);
}

TestCaseRunner::~TestCaseRunner()
{
}

TestCaseRunner* TestCaseRunner::GetActiveTestCase()
{
	return _runner;
}

void TestCaseRunner::Run()
{
	_runner = this;
	std::cout << "Running: " << Name << std::endl;
	RunTest();
}

TestApplication::TestApplication()
{
}

TestApplication::~TestApplication()
{
}

TestApplication& TestApplication::Get()
{
	static TestApplication _instance;
	return _instance;
}

void TestApplication::AddTestCase(TestCaseRunner* runner)
{
	Get().mRunners.push_back(runner);
}

void TestApplication::AddFailure(TestCaseRunner* runner, const char* file, const long line)
{
	TestFailure f;
	f.Name = runner->Name;
	f.File = file;
	f.Line = line;
	Get().mFailures.push_back(f);
}

int TestApplication::RunTests()
{
	std::cout << "STARTS RUNNING TESTS" << std::endl;
	std::cout << std::endl;

	TestApplication& app = Get();

	std::list<TestCaseRunner*>::iterator it = app.mRunners.begin();
	std::list<TestCaseRunner*>::iterator end = app.mRunners.end();
	for(; it != end; ++it) {
		(*it)->Run();
	}

	std::cout << std::endl;

	if(!app.mFailures.empty()) {
		std::cout << "TEST FAILURES" << std::endl;
		std::cout << std::endl;

		std::list<TestFailure>::iterator fit = app.mFailures.begin();
		std::list<TestFailure>::iterator fend = app.mFailures.end();
		for(; fit != fend; ++fit) {
			std::cout << "Failure: "<< (*fit).Name << std::endl  << (*fit).File << "(" << (*fit).Line << ")";
			std::cout << std::endl;
		}
		std::cout << std::endl;

		return -1;
	}

	return 0;
}
