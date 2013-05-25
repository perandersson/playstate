#pragma once

#include "../lock.h"

namespace playstate
{
	//
	// This class represents a simple lock - i.e. only a locking and unlocking mechanism exists.
	// For more advance type of locking use ConditionalLock (not implemented yet!) instead
	class SimpleLock : public ILock
	{
	public:
		SimpleLock();
		virtual ~SimpleLock();

	// ILock
	public:
		virtual void Lock();
		virtual void Unlock();

	private:
		ILock* mLock;
	};

	//
	// Class used to force unlock when this exists the current scope.
	class ScopedLock
	{
	public:
		ScopedLock(ILock* lock);
		~ScopedLock();

	private:
		ILock* mLock;
	};

}
