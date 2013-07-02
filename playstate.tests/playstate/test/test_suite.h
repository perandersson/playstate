#pragma once
#include <string>
#include <list>
#include <vector>
#include <glmock.h>

namespace playstate
{
	class UnitTest;
	class ITestCallback;

	class TestSuite
	{
		typedef std::list<UnitTest*> UnitTests;

	public:
		TestSuite(const std::string& name);
		~TestSuite();

		//
		// Run all tests located in this test-suite
		void RunUnitTests(ITestCallback* callback);

		//
		// Add a unit test in this test-suite
		// @param test
		void AddUnitTest(UnitTest* test);

	public:
		// Read-only property for this suites name
		const std::string& Name;

	private:
		UnitTests mUnitTests;
		std::string mName;
	};
}
