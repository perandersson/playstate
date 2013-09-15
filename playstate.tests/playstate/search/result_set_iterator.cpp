#include <playstate/search/result_set.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(ResultSetIterator_T)
{
	struct MockResult {} gfoundElement;

	class MockResultSet : public IResultSet<MockResult>
	{
	public:
		MockResultSet(uint32 numResults)
			: mNumResults(numResults), mNumIndex(0), mData(NULL) {
				if(numResults > 0)
					mData = (IResultSet<MockResult>::Type)malloc(numResults * sizeof(IResultSet<MockResult>::Type));
		}

		~MockResultSet() {
			if(mData != NULL)
				free(mData);
			mData = NULL;
		}

	// IResultSet
	public:
		virtual uint32 GetNumResults() const {
			return mNumResults;
		}
		virtual IResultSet<MockResult>::Type GetResultData() const {
			return mData;
		}
		virtual void Reset() {
		}

	private:
		IResultSet<MockResult>::Type mData;
		uint32 mNumResults;
		uint32 mNumIndex;
	};

	UNIT_TEST(VerfyNoNextOnEmpty)
	{
		MockResultSet resultSet(0);
		ResultSetIterator<MockResult> it(resultSet);
		ResultSetIterator<MockResult>::Type result;
		while(result = it.Next()) {
			ASSERT_FAIL("Should not have come here!");
		}
	}

	UNIT_TEST(VerifyTwoNextsOnResultSetWithTwoElements)
	{
		MockResultSet resultSet(2);
		ResultSetIterator<MockResult> it(resultSet);
		ResultSetIterator<MockResult>::Type result;
		uint32 count = 0;
		while(result = it.Next()) {
			count++;
		}

		ASSERT_EQUALS(resultSet.GetNumResults(), 2);
		ASSERT_EQUALS(count, 2);
	}
}
