#pragma once

#include <playstate/filesystem/directory.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32FileSystem;
		class Win32Directory : public IDirectory
		{
		public:
			Win32Directory(const Win32FileSystem& fileSystem);
			Win32Directory(const Win32FileSystem& fileSystem, HANDLE handle, const playstate::string& path);
			virtual ~Win32Directory();

		// IDirectory
		public:
			virtual std::vector<playstate::string> GetFiles() const;
			virtual std::vector<playstate::string> GetDirectories() const;
			virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const;
			virtual std::auto_ptr<IDirectory> OpenDirectory(const playstate::string& path) const;
			virtual const playstate::string& GetPath() const;
			virtual bool Exists() const;
		
		private:
			const Win32FileSystem& mFileSystem;

			playstate::string mPath;
			HANDLE mDirectoryHandle;
		};
	}
}
