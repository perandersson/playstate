#include "../../memory/memory.h"
#include "lock_wrapper.h"
#include "../thread_factory.h"
using namespace playstate;

SimpleLock::SimpleLock()
{
	mLock = IThreadFactory::Get().CreateLock();
}

SimpleLock::~SimpleLock()
{
	if(mLock != NULL) {
		delete mLock;
		mLock = NULL;
	}
}

void SimpleLock::Lock()
{
	mLock->Lock();
}

void SimpleLock::Unlock()
{
	mLock->Unlock();
}

//////////////////////////////////////

ScopedLock::ScopedLock(ILock* lock) : mLock(lock)
{
	assert_not_null(lock);
	mLock->Lock();
}

ScopedLock::~ScopedLock()
{
	mLock->Unlock();
}
