#pragma once
#include <string>

namespace playstate
{
	class TestSuite;

	//
	// Base class for the unit tests run in the test framework
	class UnitTest
	{
	public:
		UnitTest(const char* name, const char* file, const long line);
		virtual ~UnitTest();

	public:
		//
		// Run this test inside the supplied test suite
		// @param suite The test suite
		virtual void Run(TestSuite* suite) = 0;

	public:
		const std::string Name;
		const std::string File;
		const long Line;
	};
}
