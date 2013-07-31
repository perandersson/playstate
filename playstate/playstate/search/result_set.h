#pragma once

#include "../types.h"

namespace playstate
{
	//
	// The ResultSet base class is used when performing different types of find queries inside the the game engine.
	template<typename T>
	class ResultSet
	{
	protected:
		//
		// Default constructor
		// @param initialBlocksCount How many blocks this array contains when this instance is created.
		// @param blocksResizeCount The amount of blocks added when the array is full.
		ResultSet(uint32 initialBlocksCount, uint32 blocksResizeCount);

	public:
		virtual ~ResultSet();

		//
		// Resets this result-set. This is to preseve the allocated memory.
		void Reset();

		//
		// Creates a new instance and returns it. This increases the Size with 1.
		// @return Returns a free object from the memory pool. Resizes the array when the memory pool is full.
		T* GetOrCreate();

		T* GetElements();

		uint32 GetSize();

	protected:
		T* mMemory;
		uint32 mSize;
		uint32 mNumElements;
		uint32 mInitialBlocksCount;
		uint32 mBlocksResizeCount;
	};

	template<typename T>
	ResultSet<T>::ResultSet(uint32 initialBlocksCount, uint32 blocksResizeCount) : mMemory(0), mSize(0), mNumElements(initialBlocksCount),
		mInitialBlocksCount(initialBlocksCount), mBlocksResizeCount(blocksResizeCount)
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
	T* ResultSet<T>::GetOrCreate()
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
	
	template<typename T>
	T* ResultSet<T>::GetElements()
	{
		return mMemory;
	}
	
	template<typename T>
	uint32 ResultSet<T>::GetSize()
	{
		return mSize;
	}
}
