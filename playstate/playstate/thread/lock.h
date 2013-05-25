#pragma once

namespace playstate
{
	class ILock
	{
	public:
		virtual ~ILock() {}

		//
		// Locks this lock to prevent multiple threads to continue until it's unlocked
		virtual void Lock() = 0;

		//
		// Unlocks this thread.
		virtual void Unlock() = 0;
	};
}
