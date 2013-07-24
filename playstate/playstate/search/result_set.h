#pragma once

#include "../types.h"

namespace playstate
{
	//
	template<typename T>
	class ResultSet
	{
	public:
		//
		// Default constructor
		ResultSet(uint32 initialBlocksCount, uint32 blocksResizeCount);

		//
		// Destructor
		virtual ~ResultSet();

		//
		// Resets this result-set. This is to preseve the allocated memory.
		void Reset();

	protected:
		//
		// Creates a new instance and returns it.
		T* Create();

	public:
		// Read-only property for the number of elements allocated
		const uint32& Size;

	protected:
		T* mMemory;
		uint32 mSize;
		uint32 mNumElements;
		uint32 mInitialBlocksCount;
		uint32 mBlocksResizeCount;
	};

	template<typename T>
	ResultSet<T>::ResultSet(uint32 initialBlocksCount, uint32 blocksResizeCount) : mMemory(0), mSize(0), mNumElements(initialBlocksCount),
		mInitialBlocksCount(initialBlocksCount), mBlocksResizeCount(blocksResizeCount),
		Size(mSize)
	{
		mMemory = (T*)malloc(initialBlocksCount * sizeof(T));
	}

	template<typename T>
	ResultSet<T>::~ResultSet()
	{
		free(mMemory);
		mMemory = NULL;
		mSize = 0;
		mNumElements = 0;
	}
	
	template<typename T>
	T* ResultSet<T>::Create()
	{
		if(mSize >= mNumElements) {
			mNumElements += mBlocksResizeCount;
			mMemory = (T*)realloc(mMemory, mNumElements * sizeof(T));
		}

		const uint32 index = mSize++;
		T* ref = &mMemory[index];
		memset(ref, 0, sizeof(T));
		return ref;
	}
	
	template<typename T>
	void ResultSet<T>::Reset()
	{
		mSize = 0;
	}
}
