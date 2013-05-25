#pragma once

#include "../camera/camera.h"

namespace playstate
{
	template<typename T>
	class FindResultSet
	{
	public:
		FindResultSet() {}
		~FindResultSet() {}

		T* Next() {
			T* ptr = *mNext;
			mNext++;
			return ptr;
		}

		void SetMemory(T** memory) {
			mMemory = memory;
			mNext = memory;
		}

		void Reset() {
			mNext = mMemory;
		}

	private:
		T** mMemory;
		T** mNext;
	};
}
