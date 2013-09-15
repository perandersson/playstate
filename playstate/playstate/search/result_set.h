#pragma once

#include "../types.h"
#include "array_sorter.h"

namespace playstate
{
	template<typename T> class IResultSet;

	template<typename T>
	class ResultSetIterator
	{
	public:
		typedef typename T* Type;

	public:
		ResultSetIterator(IResultSet<T>& resultSet);
		~ResultSetIterator();

		//
		// Iterate to the next element. If a next element is found then return the object; NULL otherwise.
		Type Next();

	private:
		IResultSet<T>& mResultSet;
		uint32 mNumResults;
		uint32 mCurrentResultIndex;
		T** mResultSetData;
	};

	//
	// The ResultSet base class is used when performing different types of find queries inside the the game engine.
	template<typename T>
	class IResultSet
	{
	public:
		typedef typename T** Type;

	public:
		virtual ~IResultSet() {}
		
		//
		// Resets the ResultSet container. Useful if we want to iterate the object multiple times.
		virtual void Reset() = 0;

		//
		// @return The number of results this ResultSet contains
		virtual uint32 GetNumResults() const = 0;

		//
		// @return The result data pointers
		virtual Type GetResultData() const = 0;
	};

	////////////////////////////
	
	template<typename T>
	ResultSetIterator<T>::ResultSetIterator(IResultSet<T>& resultSet)
		: mResultSet(resultSet), mNumResults(resultSet.GetNumResults()),
		mCurrentResultIndex(0), mResultSetData(resultSet.GetResultData())
	{
	}
	
	template<typename T>
	ResultSetIterator<T>::~ResultSetIterator()
	{
		mResultSet.Reset();
	}
	
	template<typename T>
	typename ResultSetIterator<T>::Type ResultSetIterator<T>::Next()
	{
		if(mCurrentResultIndex >= mNumResults)
			return NULL;

		return mResultSetData[mCurrentResultIndex++];
	}
}
