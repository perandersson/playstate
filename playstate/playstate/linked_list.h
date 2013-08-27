#pragma once
#include <cassert>
#include "types.h"

namespace playstate
{
	template<class T> class LinkedListLink;
	template<class T> class LinkedList;

	template<class T>
	class LinkedListLink
	{
	public:
		//
		// Constructor
		LinkedListLink();

		//
		// Destructor
		virtual ~LinkedListLink();

		//
		// Unlink this node from the linked list.
		void Unlink();

		//
		// Link an item
		void Link(T* item, LinkedList<T>* list);

		//
		// @return TRUE if this link is attached to a list.
		bool IsLinked() const;

	private:
		size_t mOffset;

	public:
		T* Head;
		T* Tail;
		LinkedList<T>* List;
	};

	template<class T>
	class LinkedList
	{
		friend class LinkedListLink<T>;

	public:
		typename typedef LinkedListLink<T> Link;

	public:
		//
		// Constructor
		LinkedList(size_t offset);

		//
		// Destructor
		~LinkedList();

		// 
		// Add an item at the end of the list
		// @param item the item we want to add into this list
		void AddLast(T* item);

		//
		// Removes an item from the list
		void Remove(T* item);

		//
		// Returns the first element of this list.
		T* First() const;

		// 
		// Delete all nodes inside this list
		void DeleteAll();
	
		//
		// Unlinks all the nodes inside this list
		void UnlinkAll();

		//
		// @return The number of elements located in this list
		uint32 GetSize() const;

	protected:
		//
		// Retrieves the link value from the supplied item.
		Link* GetLink(T* item);
		
	private:
		size_t mLinkOffset;
		T* mHead;
		T* mTail;
		uint32 mSize;
	};

	/////////////////////////////////

	template<class T>
	LinkedListLink<T>::LinkedListLink() : Head(NULL), Tail(NULL), List(NULL), mOffset(0)
	{
	}

	template<class T>
	LinkedListLink<T>::~LinkedListLink()
	{
		Unlink();
	}

	template<class T>
	void LinkedListLink<T>::Unlink()
	{
		if(List != NULL) {
			// We are head if Head is NULL!
			if(Head == NULL) {
				List->mHead = Tail;
			}
		
			// We are tail if Tail is NULL!
			if(Tail == NULL) {
				List->mTail = Head;
			}

			if(Head != NULL) {
				LinkedListLink<T>* link = (LinkedListLink<T>*)((char*)(Head) + mOffset);
				link->Tail = Tail;
			}

			if(Tail != NULL) {
				LinkedListLink<T>* link = (LinkedListLink<T>*)((char*)(Tail) + mOffset);
				link->Head = Head;
			}

			List->mSize--;
		}

		List = NULL;
		Head = NULL;
		Tail = NULL;
	}

	template<class T>
	void LinkedListLink<T>::Link(T* item, LinkedList<T>* list)
	{
		// Offset is needed so that we can find where the link is located inside the item
		mOffset = (char*)(this) - (char*)(item);
		List = list;
	}

	template<class T>
	bool LinkedListLink<T>::IsLinked() const
	{
		return List != NULL;
	}

	///////////////////////////////////

	template<class T>
	LinkedList<T>::LinkedList(size_t offset) : mLinkOffset(offset), mHead(NULL), mTail(NULL), mSize(0)
	{
	}

	template<class T>
	LinkedList<T>::~LinkedList()
	{
		UnlinkAll();
	}

	template<class T>
	void LinkedList<T>::AddLast(T* item)
	{
		// Find the link for the supplied item
		Link* link = GetLink(item);
		
		// Make sure that the items link isn't attached to the list
		link->Unlink();

		// Assign the item into the linked list
		if(mHead == NULL) {
			mHead = mTail = item;
 		} else {
			// Put the item to the end of the list
			GetLink(mTail)->Tail = item;
			link->Head = mTail;
			mTail = item;
		}

		// Link the item with this list
		link->Link(item, this);
		mSize++;
	}

	template<class T>
	void LinkedList<T>::Remove(T* item) 
	{
		assert(item != NULL && "You cannot remove a non-existing item");

		Link* link = GetLink(item);
		if(link->List == NULL)
			return;

		assert(link->List == this && "You cannot remove another lists nodes");

		link->Unlink();
	}

	template<class T>
	typename LinkedList<T>::Link* LinkedList<T>::GetLink(T* item)
	{
		char* mem = reinterpret_cast<char*>(item) + mLinkOffset;
		return reinterpret_cast<Link*>(mem);
	}

	template<class T>
	T* LinkedList<T>::First() const
	{
		return mHead;
	}

	template<class T>
	void LinkedList<T>::UnlinkAll() {
		T* ptr = mHead;
		while(ptr != NULL) {
			Link* link = GetLink(ptr);
			T* next = link->Tail;
			link->Head = link->Tail = NULL;
			link->List = NULL;
			ptr = next;
		}
		mHead = mTail = NULL;
	}

	template<class T>
	void LinkedList<T>::DeleteAll()
	{
		T* ptr = mHead;
		while(ptr != NULL) {
			Link* link = GetLink(ptr);
			T* next = link->Tail;
			delete ptr;
			ptr = next;
		}
		mHead = mTail = NULL;
	}

	template<class T>
	uint32 LinkedList<T>::GetSize() const
	{
		return mSize;
	}

}
