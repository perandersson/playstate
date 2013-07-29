#include <playstate/memory/memory.h>
#include "win32_file.h"
#include "win32_directory.h"
#include "win32_filesystem.h"

#include <cassert>

using namespace playstate;
using namespace playstate::win32;

Win32File::Win32File(const Win32FileSystem& fileSystem) 
	: mFileSystem(fileSystem), mFileSize(0), mFileHandle(NULL)
{
}

Win32File::Win32File(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path) 
	:  mFileSystem(fileSystem), mPath(path), mFileSize(0), mFileHandle(handle)
{
	mFileSize = GetFileSize(mFileHandle, NULL);
}

Win32File::~Win32File()
{
	if(mFileHandle != NULL) {
		CloseHandle(mFileHandle);
		mFileHandle = NULL;
	}

	mPath.clear();
}

std::istringstream Win32File::Read() const
{
	std::istringstream stream;
	if(Exists()) {
		byte* buffer = new byte[mFileSize + 1];
		ReadBytes(buffer, mFileSize);
		buffer[mFileSize] = 0;
		stream = std::istringstream(std::string((char*)buffer));
		delete[] buffer;
	}

	return stream;
}

std::auto_ptr<IFile> Win32File::OpenFile(const std::string& path) const
{
	if(!Exists())
		return std::auto_ptr<IFile>(new Win32File(mFileSystem));
	
	std::string fullPath = path;
	if(mFileSystem.IsRelative(path)) {
		std::string::size_type pos = mPath.find_last_of(L'/');
		std::string pathToDir = mPath.substr(0, pos + 1);
		fullPath = pathToDir + path;
	}

	return mFileSystem.OpenFile(fullPath);
}

uint64 Win32File::Size() const
{
	return mFileSize;
}

uint64 Win32File::ReadBytes(byte* buffer, uint64 offset, uint64 bufferSize) const
{
	if(!Exists())
		return 0;

	DWORD bytesRead = 0;
	OVERLAPPED ol = {0};
	ol.Offset = LOWORD(offset);
	ol.OffsetHigh = HIWORD(offset);
	if(ReadFile(mFileHandle, buffer, bufferSize, &bytesRead, &ol))
		return (uint64)bytesRead;

	return 0;
}

uint64 Win32File::ReadBytes(byte* buffer, uint64 bufferSize) const
{
	return ReadBytes(buffer, 0, bufferSize);
}

const std::string& Win32File::GetPath() const
{
	return mPath;
}

std::auto_ptr<IDirectory> Win32File::GetDirectory() const
{
	if(!Exists())
		return std::auto_ptr<IDirectory>(new Win32Directory(mFileSystem));

	std::string::size_type pos = mPath.find_last_of(L'/');
	std::string pathToDir = mPath.substr(0, pos);
	return mFileSystem.OpenDirectory(pathToDir);
}

bool Win32File::Exists() const
{
	return mFileHandle != NULL;
}
