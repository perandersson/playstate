#include <playstate/linked_list.h>
#include "../test.h"
using namespace playstate;

class DummyClass
{
public:
	LinkedListLink<DummyClass> Link;
};

class DummyContainer
{
public:
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
	LinkedList<DummyClass, &DummyClass::Link> mDummies;
};

TestCase(LinkedList_RemoveItemFromListOnDelete, "delete -> removes item from list")
{
	DummyContainer container;
	DummyClass* item = new DummyClass();

	container.Add(item);

	AssertEquals(container.GetSize(), 1);

	delete item;

	AssertEquals(container.GetSize(), 0);
}
