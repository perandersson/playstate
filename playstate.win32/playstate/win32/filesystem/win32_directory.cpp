#include <playstate/memory/memory.h>
#include "win32_directory.h"
#include "win32_file.h"
#include "win32_filesystem.h"
#include <cassert>

using namespace playstate;
using namespace playstate::win32;

Win32Directory::Win32Directory(const Win32FileSystem& fileSystem) 
	: mFileSystem(fileSystem), mDirectoryHandle(NULL)
{
}

Win32Directory::Win32Directory(const Win32FileSystem& fileSystem, HANDLE handle, const std::string& path) 
	: mFileSystem(fileSystem), mPath(path), mDirectoryHandle(handle)
{
}

Win32Directory::~Win32Directory()
{
	if(mDirectoryHandle != NULL) {
		CloseHandle(mDirectoryHandle);
		mDirectoryHandle = NULL;
	}

	mPath.clear();
}

std::vector<std::string> Win32Directory::GetFiles() const
{
	std::vector<std::string> files;
	/*WIN32_FIND_DATAA ffd;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	
	fileHandle = FindFirstFileA(mPath.c_str(), &ffd);
	if(fileHandle == INVALID_HANDLE_VALUE) {
		return files;
	}

	do {
		files.push_back(ffd.cFileName);
	} while(FindNextFileA(fileHandle, &ffd) != 0);

	FindClose(fileHandle);*/
	return files;
}

std::vector<std::string> Win32Directory::GetDirectories() const
{
	std::vector<std::string> directories;
	
	return directories;
}

std::auto_ptr<IFile> Win32Directory::OpenFile(const std::string& path) const
{
	if(!Exists())
		return std::auto_ptr<IFile>(new Win32File(mFileSystem));

	std::string fullPath = path;
	if(mFileSystem.IsRelative(path)) {
		fullPath = mPath + "/" + path;
	}

	return mFileSystem.OpenFile(fullPath);
}

std::auto_ptr<IDirectory> Win32Directory::OpenDirectory(const std::string& path) const
{
	if(!Exists())
		return std::auto_ptr<IDirectory>(new Win32Directory(mFileSystem));
	
	std::string fullPath = path;
	if(mFileSystem.IsRelative(path)) {
		fullPath = mPath + path;
	}
	
	return mFileSystem.OpenDirectory(fullPath);
}

const std::string& Win32Directory::GetPath() const
{
	return mPath;
}

bool Win32Directory::Exists() const
{
	return mDirectoryHandle != NULL;
}
