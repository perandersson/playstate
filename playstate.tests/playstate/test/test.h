#pragma once
#include "test_suite.h"
#include "test_callback.h"
#include "test_runner.h"
#include "unit_test.h"
#include "test_exception.h"

namespace playstate
{
	#define TEST_SUITE(Name) \
		namespace Name##_suite { static const char SuiteName[] = {#Name}; } \
		namespace Name##_suite
	
	#define UNIT_TEST(Name) \
		void Name##_runner(TestSuite* suite, UnitTest* unitTest); \
		class Name##_UnitTest : public UnitTest { \
		public: \
			Name##_UnitTest() : UnitTest(#Name, __FILE__, __LINE__) { \
				TestRunner::AddUnitTest(SuiteName, this); \
			} \
			virtual void Run(TestSuite* suite) { Name##_runner(suite, this); } \
		} _##Name##_UnitTest_instance; \
		void Name##_runner(TestSuite* suite, UnitTest* unitTest)

	#define ASSERT_EXPRESSION(Expression, Message) \
		if((Expression)) { \
			throw TestException(unitTest, __FILE__, Message, __LINE__); \
		}

	#define ASSERT_TRUE(Condition) ASSERT_EXPRESSION(Condition == false, "Value is false")
	#define ASSERT_FALSE(Condition) ASSERT_EXPRESSION(Condition == true, "Value is true")
	#define ASSERT_EQUALS(Value, Expected) ASSERT_EXPRESSION(Value != Expected, "Value are not equal")
	#define ASSERT_NOT_EQUALS(Value, Expected) ASSERT_EXPRESSION(Value == Expected, "Value are equal")
	#define ASSERT_NOT_NULL(Value) ASSERT_EXPRESSION(Value == NULL, "Value is null")
	#define ASSERT_NULL(Value) ASSERT_EXPRESSION(Value != NULL, "Value is not null")
	#define ASSERT_FAIL(Message) ASSERT_EXPRESSION(true, Message)

//	#define GET_OPENGL_MOCK() glmock::Create(suite)
//	#define VALIDATE_OPENGL_MOCK(mock) glmock::Destroy(mock)

	// Global test-suite name.
	// @remark Use the TEST_SUITE macro for grouping unit tests together.
	static const char SuiteName[] = {"Global Suite"};
}
