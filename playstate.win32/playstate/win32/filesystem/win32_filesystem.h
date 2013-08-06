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
			typedef std::vector<std::string> Paths;

		public:
			Win32FileSystem(const std::string& path);
			Win32FileSystem(const std::vector<std::string>& paths);

			//
			// Destructor
			virtual ~Win32FileSystem();

			//
			// Check if a path is relative or not
			// @return TRUE if the supplied path is a relative path; FALSE otherwise.
			bool IsRelative(const std::string& path) const;

			//
			// Retrieves the Windows file handle for the supplied file that has the highest priority
			// @param path
			HANDLE GetHighestPriorityPathHandle(const std::string& path) const;

			//
			// Poll this implementation for file changes
			void Poll();

		// IFileSystem
		public:
			virtual std::auto_ptr<IFile> OpenFile(const std::string& path) const;
			virtual std::auto_ptr<IDirectory> OpenRootDir() const;
			virtual std::auto_ptr<IDirectory> OpenDirectory(const std::string& path) const;
			virtual void AddLookupDirectory(const std::string& directory);
			virtual void RemoveLookupDirectory(const std::string& directory);
			virtual void AddFileChangedListener(const std::regex& regex, IFileChangedListener* listener);
			virtual void AddFileChangedListener(const std::string& path, IFileChangedListener* listener);
			virtual void RemoveFileChangedListener(IFileChangedListener* listener);

		private:
			Paths mPaths;
			Win32FileWatcher mFileWatcher;
		};
	}
}
