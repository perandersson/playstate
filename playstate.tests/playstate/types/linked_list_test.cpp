#include <playstate/linked_list.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(LinkedList)
{
	class DummyClass
	{
	public:
		LinkedListLink<DummyClass> Link;
	};

	class DummyContainer
	{
	public:
		DummyContainer()
			: mDummies(offsetof(DummyClass, Link))
		{
		}

		~DummyContainer()
		{
		}

		void Add(DummyClass* dummy)
		{
			mDummies.AddLast(dummy);
		}

		int GetSize()
		{
			int count = 0;
			DummyClass* item = mDummies.First();
			while(item != NULL) {
				count++;
				item = item->Link.Tail;
			}
			return count;
		}

	private:
		LinkedList<DummyClass> mDummies;
	};

	UNIT_TEST(LinkedList_RemoveItemFromListOnDelete)
	{
		DummyContainer container;
		DummyClass* item = new DummyClass();

		container.Add(item);

		ASSERT_EQUALS(container.GetSize(), 1);

		delete item;

		ASSERT_EQUALS(container.GetSize(), 0);
	}
}
