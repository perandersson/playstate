#include <playstate/memory/memory.h>
#include "win32_file.h"
#include "win32_directory.h"
#include "win32_filesystem.h"
#include <algorithm>

using namespace playstate;
using namespace playstate::win32;

Win32FileSystem::Win32FileSystem(const std::string& path)
	: mPaths()
{
	mPaths.push_back(path);
}

Win32FileSystem::Win32FileSystem(const std::vector<std::string>& paths)
	: mPaths(paths)
{
}

Win32FileSystem::~Win32FileSystem()
{
}

std::auto_ptr<IFile> Win32FileSystem::OpenFile(const std::string& path) const
{
	// Unknown if the path is relative - must start with a '/'
	if(IsRelative(path))
		return std::auto_ptr<IFile>(new Win32File(*this));

	HANDLE fileHandle = GetHighestPriorityPathHandle(path);
	if(fileHandle != NULL)
		return std::auto_ptr<IFile>(new Win32File(*this, fileHandle, path));
	else
		return std::auto_ptr<IFile>(new Win32File(*this));
}

std::auto_ptr<IDirectory> Win32FileSystem::OpenRootDir() const
{
	std::string path = "/";
	HANDLE fileHandle = GetHighestPriorityPathHandle(path);
	return std::auto_ptr<IDirectory>(new Win32Directory(*this, fileHandle, path));
}

std::auto_ptr<IDirectory> Win32FileSystem::OpenDirectory(const std::string& path) const
{
	// Unknown if the path is relative - must start with a '/'
	if(IsRelative(path))
		return std::auto_ptr<IDirectory>(new Win32Directory(*this));
	
	HANDLE fileHandle = GetHighestPriorityPathHandle(path);
	if(fileHandle != NULL)
		return std::auto_ptr<IDirectory>(new Win32Directory(*this, fileHandle, path));
	else
		return std::auto_ptr<IDirectory>(new Win32Directory(*this));
}

HANDLE Win32FileSystem::GetHighestPriorityPathHandle(const std::string& path) const
{
	std::vector<std::string>::size_type size = mPaths.size();
	for(std::vector<std::string>::size_type i = 0; i < mPaths.size(); ++i) {
		std::string absolutePath = mPaths[i] + path;
		HANDLE handle = CreateFile(absolutePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(handle == INVALID_HANDLE_VALUE) {
			// Try as a folder
			handle = CreateFile(absolutePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
				NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
		}

		if(handle != INVALID_HANDLE_VALUE) {
			return handle;
		}
	}

	return NULL;
}

bool Win32FileSystem::IsRelative(const std::string& path) const
{
	if(path.empty())
		return true;

	return path.c_str()[0] != L'/';
}

void Win32FileSystem::AddLookupDirectory(const std::string& path)
{
	mPaths.push_back(path);
}

void Win32FileSystem::RemoveLookupDirectory(const std::string& path)
{
	Paths::iterator it = std::find(mPaths.begin(), mPaths.end(), path);
	if(it != mPaths.end())
		mPaths.erase(it);
}