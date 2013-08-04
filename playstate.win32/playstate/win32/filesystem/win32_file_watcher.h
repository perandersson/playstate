#pragma once

#include <playstate/filesystem/file_system.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "../thread/win32_thread.h"
#include "../thread/win32_lock.h"

namespace playstate
{
	namespace win32
	{
		struct Win32FileListener	
		{
			Win32FileListener() {}
			~Win32FileListener() {}

			IFileChangedListener* Callback;
			std::string FileName;
		};

		struct Win32FileEvent
		{
			Win32FileEvent() {}
			~Win32FileEvent() {}

			IFileChangedListener* Callback;
			std::string FileName;
			FileChangeAction::Enum Action;
		};

		class Win32FileWatcher : public IRunnable
		{
			typedef std::list<Win32FileListener*> FileChangedListeners;
			typedef std::vector<Win32FileEvent*> FileEvents;

		public:
			Win32FileWatcher(const std::string& rootDirPath);
			~Win32FileWatcher();

			//
			// Refresh this file watcher and look file changes in the file system
			//
			void LookForChanges();

			void AddListener(const std::string& fileName, IFileChangedListener* listener);

			void RemoveListener(IFileChangedListener* listener);

			void GetAndClearEvents(FileEvents& events);

		// IRunnable
		public:
			virtual void Run(IThread& thread);

		private:
			FileChangedListeners mFileChangedListeners;
			Win32Lock* mListenersLock;

			HANDLE mRootDirHandle;
			Win32Thread* mThread;

			FileEvents mFileEvents;
			Win32Lock* mFileEventsLock;
		};
	}
}
