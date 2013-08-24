#pragma once

#include <playstate/filesystem/file.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32FileSystem;
		class Win32File : public IFile
		{
		public:
			Win32File(const Win32FileSystem& fileSystem);
			Win32File(const Win32FileSystem& fileSystem, HANDLE handle, const playstate::string& path);
			virtual ~Win32File();

		// IFile
		public:
			virtual std::istringstream Read() const;
			virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const;
			virtual uint32 Size() const;
			virtual uint32 ReadBytes(byte* buffer, uint32 offset, uint32 bufferSize) const;
			virtual uint32 ReadBytes(byte* buffer, uint32 bufferSize) const;
			virtual const playstate::string& GetPath() const;
			virtual std::auto_ptr<IDirectory> GetDirectory() const;
			virtual bool Exists() const;
		
		private:
			const Win32FileSystem& mFileSystem;

			playstate::string mPath;
			uint32 mFileSize;
			HANDLE mFileHandle;
		};
	}
}
