#pragma once

#include "../directory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	class Win32FileSystem;
	class Win32Directory : public IDirectory
	{
	public:
		Win32Directory(const Win32FileSystem& fileSystem);
		Win32Directory(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path);
		virtual ~Win32Directory();

	// IDirectory
	public:
		virtual std::vector<std::string> GetFiles() const;
		virtual std::vector<std::string> GetDirectories() const;
		virtual std::auto_ptr<IFile> OpenFile(const std::string& path) const;
		virtual std::auto_ptr<IDirectory> OpenDirectory(const std::string& path) const;
		virtual const std::string& GetPath() const;
		virtual bool Exists() const;
		
	private:
		const Win32FileSystem& mFileSystem;

		std::string mPath;
		HANDLE mDirectoryHandle;
	};
}
