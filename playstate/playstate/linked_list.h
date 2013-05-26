#pragma once
#include <cassert>
#ifndef NULL
#define NULL 0
#endif

namespace playstate
{
	template<class T>
	class LinkedListBase
	{
	public:
		LinkedListBase();
		virtual ~LinkedListBase();
	
	public:
		T* Head;
		T* Tail;
	};

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
		void Link(T* item, LinkedListBase<T>* list);

	private:
		size_t mOffset;

	public:
		T* Head;
		T* Tail;
		LinkedListBase<T>* List;
	};

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	class LinkedList : public LinkedListBase<T>
	{
	public:
		typename typedef LinkedListLink<T> Link;

	public:
		//
		// Constructor
		LinkedList();

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

	protected:
		//
		// Retrieves the link value from the supplied item.
		Link& GetLink(T* item);
	
	};

	///////////////////////////////////

	template<class T>
	LinkedListBase<T>::LinkedListBase() : Head(NULL), Tail(NULL)
	{
	}

	template<class T>
	LinkedListBase<T>::~LinkedListBase()
	{
	}

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
				List->Head = Tail;
			}
		
			// We are tail if Tail is NULL!
			if(Tail == NULL) {
				List->Tail = Head;
			}
		}

		if(Head != NULL) {
			LinkedListLink<T>* link = (LinkedListLink<T>*)((char*)(Head) + mOffset);
			link->Tail = Tail;
		}

		if(Tail != NULL) {
			LinkedListLink<T>* link = (LinkedListLink<T>*)((char*)(Tail) + mOffset);
			link->Head = Head;
		}
	}

	template<class T>
	void LinkedListLink<T>::Link(T* item, LinkedListBase<T>* list)
	{
		// Offset is needed so that we can find where the link is located inside the item
		mOffset = (char*)(this) - (char*)(item);
		List = list;
	}

	///////////////////////////////////

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	LinkedList<T, _LinkAddr>::LinkedList() : LinkedListBase<T>()
	{
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	LinkedList<T, _LinkAddr>::~LinkedList()
	{
		UnlinkAll();
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	void LinkedList<T, _LinkAddr>::AddLast(T* item)
	{
		// Find the link for the supplied item
		Link& link = GetLink(item);
		
		// Make sure that the items link isn't attached to the list
		link.Unlink();

		// Assign the item into the linked list
		if(Head == NULL) {
			Head = Tail = item;
 		} else {
			// Put the item to the end of the list
			GetLink(Tail).Tail = item;
			link.Head = Tail;
			Tail = item;
		}

		// Link the item with this list
		link.Link(item, this);
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	void LinkedList<T, _LinkAddr>::Remove(T* item) 
	{
		assert(item != NULL && "You cannot remove a non-existing item");

		Link& link = GetLink(item);
		if(link.List == NULL)
			return;

		assert(link.List == this && "You cannot remove another lists nodes");

		link.Unlink();
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	typename LinkedList<T, _LinkAddr>::Link& LinkedList<T, _LinkAddr>::GetLink(T* item)
	{
		return (*item).*_LinkAddr;
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	T* LinkedList<T, _LinkAddr>::First() const
	{
		return Head;
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	void LinkedList<T, _LinkAddr>::UnlinkAll() {
		T* ptr = Head;
		while(ptr != NULL) {
			Link& link = GetLink(ptr);
			T* next = link.Tail;
			link.Head = link.Tail = NULL;
			link.List = NULL;
			ptr = next;
		}
		Head = Tail = NULL;
	}

	template<class T, typename LinkedListLink<T> T::*_LinkAddr>
	void LinkedList<T, _LinkAddr>::DeleteAll()
	{
		T* ptr = Head;
		while(ptr != NULL) {
			Link& link = GetLink(ptr);
			T* next = link.Tail;
			delete ptr;
			ptr = next;
		}
		Head = Tail = NULL;
	}
}
