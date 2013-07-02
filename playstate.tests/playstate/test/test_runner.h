#pragma once
#include <map>

namespace playstate
{
	class TestSuite;
	class UnitTest;
	class ITestCallback;

	//
	// 
	class TestRunner
	{
		typedef std::map<std::string, TestSuite*> TestSuites;

	public:
		// Destructor
		~TestRunner();

	protected:
		// Constructor
		TestRunner();

		//
		static TestRunner& Get();

	public:
		//
		// @return The return code 
		static int Run();

		//
		// @return The return code 
		// @param callback
		static int Run(ITestCallback* callback);

		//
		// Adds a unit test in the supplied suite
		// @param suiteName The name of the test suite
		// @param unitTest
		static void AddUnitTest(const char* suiteName, UnitTest* unitTest);

	private:
		TestSuites mSuites;
	};
}
