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

Win32File::Win32File(const Win32FileSystem& fileSystem, HANDLE handle, const playstate::string& path) 
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
		stream = std::istringstream(playstate::string((char*)buffer));
		delete[] buffer;
	}

	return stream;
}

std::auto_ptr<IFile> Win32File::OpenFile(const playstate::string& path) const
{
	if(!Exists())
		return std::auto_ptr<IFile>(new Win32File(mFileSystem));
	
	playstate::string fullPath = path;
	if(mFileSystem.IsRelative(path)) {
		playstate::string::size_type pos = mPath.find_last_of(L'/');
		playstate::string pathToDir = mPath.substr(0, pos + 1);
		fullPath = pathToDir + path;
	}

	return mFileSystem.OpenFile(fullPath);
}

uint32 Win32File::Size() const
{
	return mFileSize;
}

uint32 Win32File::ReadBytes(byte* buffer, uint32 offset, uint32 bufferSize) const
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

uint32 Win32File::ReadBytes(byte* buffer, uint32 bufferSize) const
{
	return ReadBytes(buffer, 0, bufferSize);
}

const playstate::string& Win32File::GetPath() const
{
	return mPath;
}

std::auto_ptr<IDirectory> Win32File::GetDirectory() const
{
	if(!Exists())
		return std::auto_ptr<IDirectory>(new Win32Directory(mFileSystem));

	playstate::string::size_type pos = mPath.find_last_of(L'/');
	playstate::string pathToDir = mPath.substr(0, pos);
	return mFileSystem.OpenDirectory(pathToDir);
}

bool Win32File::Exists() const
{
	return mFileHandle != NULL;
}
