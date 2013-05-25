#pragma once
#include <cassert>

namespace playstate
{
	template<class T>
	class Singleton
	{
	protected:
		static T* gSingleton;

	public:
		Singleton() {
			assert(gSingleton == 0);
			gSingleton = static_cast<T*>(this);
		}

		virtual ~Singleton() {
			gSingleton = 0;
		}

		static T& Get() {
			return *gSingleton;
		}
	};
}
