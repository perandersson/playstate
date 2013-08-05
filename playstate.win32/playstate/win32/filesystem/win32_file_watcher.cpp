#include <playstate/memory/memory.h>
#include "win32_file_watcher.h"
#include "win32_file.h"
#include "win32_filesystem.h"
#include <playstate/thread/wrapper/lock_wrapper.h>
#include <algorithm>

using namespace playstate;
using namespace playstate::win32;

Win32FileWatcher::Win32FileWatcher(const std::string& rootDirPath)
	: mRootDirHandle(INVALID_HANDLE_VALUE), mThread(NULL), mListenersLock(NULL), mFileEventsLock(NULL)
{
	mRootDirHandle = CreateFile(rootDirPath.c_str(), GENERIC_READ, 
		FILE_LIST_DIRECTORY | FILE_SHARE_READ |FILE_SHARE_WRITE |FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
	if(mRootDirHandle == INVALID_HANDLE_VALUE) {
		// TODO Add exception
		return;
	}

	mListenersLock = new Win32Lock();
	mFileEventsLock = new Win32Lock();

	mThread = new Win32Thread(this);
	mThread->Start();
}

Win32FileWatcher::~Win32FileWatcher()
{
	HANDLE oldHandle = mRootDirHandle;
	mRootDirHandle = INVALID_HANDLE_VALUE;
	CloseHandle(oldHandle);

	delete mThread;
	delete mFileEventsLock;
	delete mListenersLock;
}

void Win32FileWatcher::LookForChanges()
{
	FileEvents events;
	GetAndClearEvents(events);
	if(!events.empty()) {
		FileEvents::size_type size = events.size();
		for(FileEvents::size_type i = 0; i < size; ++i) {
			Win32FileEvent* fileEvent = events[i];

			std::auto_ptr<IFile> file = IFileSystem::Get().OpenFile(fileEvent->FileName);
			fileEvent->Callback->FileChanged(*file.get(), fileEvent->Action);
			delete fileEvent;
		}
	}
}

void Win32FileWatcher::GetAndClearEvents(FileEvents& events)
{
	if(mFileEvents.empty())
		return;

	ScopedLock sl(mFileEventsLock);

	events = FileEvents(mFileEvents);
	mFileEvents.clear();
}

void Win32FileWatcher::AddListener(const std::string& fileName, IFileChangedListener* listener)
{
	ScopedLock s(mListenersLock);

	Win32FileListener* fileListener = new Win32FileListener();
	fileListener->Callback = listener;
	fileListener->FileName = fileName;
	mFileChangedListeners.push_back(fileListener);
}

void Win32FileWatcher::RemoveListener(IFileChangedListener* listener)
{
	ScopedLock s(mListenersLock);

	FileChangedListeners::iterator it = mFileChangedListeners.begin();
	FileChangedListeners::iterator end = mFileChangedListeners.end();
	std::vector< FileChangedListeners::iterator  > listenersToRemove;
	for(; it != end; ++it) {
		Win32FileListener* fileListener = (*it);
		if(fileListener->Callback == listener) {
			listenersToRemove.push_back(it);
			delete fileListener;
		}
	}
	
	for(unsigned int i = 0; i < listenersToRemove.size(); ++i) {
		mFileChangedListeners.erase(listenersToRemove[i]);
	}
}

void Win32FileWatcher::Run(IThread& thread)
{
	BYTE bytes[32 * 1024];
	DWORD bytesret;
	TCHAR szFile[MAX_PATH];
	
	while(ReadDirectoryChangesW(mRootDirHandle, bytes, sizeof(bytes), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &bytesret, NULL, NULL) == TRUE
		&& mRootDirHandle != INVALID_HANDLE_VALUE) {

		PFILE_NOTIFY_INFORMATION pNotify;
		DWORD offset = 0;
		do {
			pNotify = (PFILE_NOTIFY_INFORMATION) &bytes[offset];
			offset += pNotify->NextEntryOffset;

			FileChangeAction::Enum action = FileChangeAction::MODIFIED;
			if(pNotify->Action == FILE_ACTION_ADDED) {
				action = FileChangeAction::ADDED;
			} else if(pNotify->Action == FILE_ACTION_REMOVED) {
				action = FileChangeAction::DELETED;
			}

			int count = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / sizeof(WCHAR),
										szFile, MAX_PATH - 1, NULL, NULL);
			szFile[count] = 0;
			std::string fileName = "/";
			fileName += szFile;
			std::replace(fileName.begin(), fileName.end(), '\\', '/');
			
			FileChangedListeners::iterator it = mFileChangedListeners.begin();
			FileChangedListeners::iterator end = mFileChangedListeners.end();
			for(;it != end; ++it) {
				Win32FileListener* listener = *it;
				if(listener->FileName == fileName) {
					ScopedLock sl(mFileEventsLock);
					Win32FileEvent* fileEvent = new Win32FileEvent();
					fileEvent->Action = action;
					fileEvent->Callback = listener->Callback;
					fileEvent->FileName = listener->FileName;
					mFileEvents.push_back(fileEvent);
				}
			}
		} while(pNotify->NextEntryOffset != 0);
		thread.Wait(800);
	}
}
