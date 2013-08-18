#pragma once

#include <playstate/filesystem/file_system.h>
#include <playstate/types.h>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "win32_file_watcher.h"

namespace playstate
{
	namespace win32
	{
		class Win32FileSystem : public IFileSystem
		{
			typedef std::vector<playstate::string> Paths;

		public:
			Win32FileSystem(const playstate::string& path);
			Win32FileSystem(const std::vector<playstate::string>& paths);

			//
			// Destructor
			virtual ~Win32FileSystem();

			//
			// Check if a path is relative or not
			// @return TRUE if the supplied path is a relative path; FALSE otherwise.
			bool IsRelative(const playstate::string& path) const;

			//
			// Retrieves the Windows file handle for the supplied file that has the highest priority
			// @param path
			HANDLE GetHighestPriorityPathHandle(const playstate::string& path) const;

			//
			// Poll this implementation for file changes
			void Poll();

		// IFileSystem
		public:
			virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const;
			virtual std::auto_ptr<IDirectory> OpenRootDir() const;
			virtual std::auto_ptr<IDirectory> OpenDirectory(const playstate::string& path) const;
			virtual void AddLookupDirectory(const playstate::string& directory);
			virtual void RemoveLookupDirectory(const playstate::string& directory);
			virtual void AddFileChangedListener(const playstate::regex& regex, IFileChangedListener* listener);
			virtual void AddFileChangedListener(const playstate::string& path, IFileChangedListener* listener);
			virtual void RemoveFileChangedListener(IFileChangedListener* listener);

		private:
			Paths mPaths;
			Win32FileWatcher mFileWatcher;
		};
	}
}
