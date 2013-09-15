#include <playstate/search/result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(ResultSetIterator_T)
{
	struct MockResult {};

	class MockResultSet : public IResultSet<MockResult>
	{
	// IResultSet
	public:
	public:
		virtual uint32 GetNumResults() const {
			return 0;
		}
		virtual IResultSet<MockResult>::Type GetResultData() const {
			return NULL;
		}
		virtual void Reset() {
		}

	private:
	};

	UNIT_TEST(VerfyNoNextOnEmpty)
	{
		MockResultSet resultSet;
		ResultSetIterator<MockResult> it(resultSet);
		ResultSetIterator<MockResult>::Type result;
		while(result = it.Next()) {
			ASSERT_FAIL("Should not have come here!");
		}
	}
}
