#pragma once
#include "../types.h"

namespace playstate
{
	//
	//
	template<typename ArrayT>
	class IArraySorter
	{
	public:
		virtual ~IArraySorter() {}

	public:
		//
		// Sorts the supplied items list
		// @param items
		// @param numItems
		virtual void Sort(ArrayT* items, uint32 numItems) = 0; 
	};

}
