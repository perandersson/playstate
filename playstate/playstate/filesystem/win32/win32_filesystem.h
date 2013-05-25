#pragma once

#include "../file_system.h"
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace playstate
{
	class Win32FileSystem : public IFileSystem
	{
		typedef std::vector<std::string> Paths;

	public:
		//
		// Constructor
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

	// IFileSystem
	public:
		virtual std::auto_ptr<IFile> OpenFile(const std::string& path) const;
		virtual std::auto_ptr<IDirectory> OpenRootDir() const;
		virtual std::auto_ptr<IDirectory> OpenDirectory(const std::string& path) const;
		virtual void AddLookupDirectory(const std::string& directory);
		virtual void RemoveLookupDirectory(const std::string& directory);

	private:
		Paths mPaths;
	};

}
