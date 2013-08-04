#pragma once

#include "../singleton.h"
#include "file.h"
#include "directory.h"
#include "file_changed_listener.h"

#include <memory>

namespace playstate
{
	//
	// 
	class IFileSystem : public Singleton<IFileSystem>
	{
	public:
		//
		// Tries to open a file for the supplied absolute path.
		// @param path the absolute path to the file.
		virtual std::auto_ptr<IFile> OpenFile(const std::string& path) const = 0;
		
		//
		// Retrieves the root directory.
		virtual std::auto_ptr<IDirectory> OpenRootDir() const = 0;

		//
		// Opens a directory for the supplied absolute path.
		// @param path
		virtual std::auto_ptr<IDirectory> OpenDirectory(const std::string& path) const = 0;

		//
		// Adds a directory that the filesystem uses when looking for files
		// @param directory
		virtual void AddLookupDirectory(const std::string& directory) = 0;

		//
		// Removes a directory where the filesystem looks for files
		// @param directory
		virtual void RemoveLookupDirectory(const std::string& directory) = 0;

		//
		// Adds a event listener for when a specific file is changed
		//
		// @param path
		// @param listener
		virtual void AddFileChangedListener(const std::string& path, IFileChangedListener* listener) = 0;
		
		//
		// Removes an event listener for when a specific file is changed
		//
		// @param listener
		virtual void RemoveFileChangedListener(IFileChangedListener* listener) = 0;
	};
}
