#pragma once
#include <string>
#include <list>

namespace playstate
{
	class TestCaseRunner
	{
	public:
		static TestCaseRunner* GetActiveTestCase();

		TestCaseRunner(const char* name, const char* description, const char* file, const long line);
		virtual ~TestCaseRunner();

		//
		// Run this test
		void Run();

	public:
		const std::string Name;
		const std::string Description;
		const std::string File;
		const long Line;

	protected:

		//
		// Run this test
		virtual void RunTest() = 0;
	};

	struct TestFailure
	{
		std::string Name;
		std::string File;
		long Line;
	};

	class TestApplication
	{
	public:
		TestApplication();
		~TestApplication();

	public:
		static int RunTests();
		static void AddTestCase(TestCaseRunner* runner);
		static void AddFailure(TestCaseRunner* runner, const char* file, const long line);

	private:
		static TestApplication& Get();

	private:
		std::list<TestCaseRunner*> mRunners;
		std::list<TestFailure> mFailures;
	};
	
#define TestCase(Name, Description) \
	void _func_##Name##_runner(); \
	class Name##_TestCaseRunner : public TestCaseRunner { \
	public: \
		Name##_TestCaseRunner() : TestCaseRunner(#Name, Description, __FILE__, __LINE__) {} \
		virtual void RunTest() { _func_##Name##_runner(); } \
	} _##Name##_runner; \
	void _func_##Name##_runner()


#define AssertTrue(Condition) \
	if(Condition == false) TestApplication::AddFailure(TestCaseRunner::GetActiveTestCase(), __FILE__, __LINE__);

#define AssertFalse(Condition) \
	if(Condition == true) TestApplication::AddFailure(TestCaseRunner::GetActiveTestCase(), __FILE__, __LINE__);
}
