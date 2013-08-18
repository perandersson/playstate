#pragma once

#include "../singleton.h"
#include "file.h"
#include "directory.h"
#include "file_changed_listener.h"

#include <memory>
#include <regex>

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
		virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const = 0;
		
		//
		// Retrieves the root directory.
		virtual std::auto_ptr<IDirectory> OpenRootDir() const = 0;

		//
		// Opens a directory for the supplied absolute path.
		// @param path
		virtual std::auto_ptr<IDirectory> OpenDirectory(const playstate::string& path) const = 0;

		//
		// Adds a directory that the filesystem uses when looking for files
		// @param directory
		virtual void AddLookupDirectory(const playstate::string& directory) = 0;

		//
		// Removes a directory where the filesystem looks for files
		// @param directory
		virtual void RemoveLookupDirectory(const playstate::string& directory) = 0;

		//
		// Adds a event listener for when a specific file within a regular expression file is is changed
		//
		// @param regex A regular expression of the file we want to listen to
		// @param listener
		virtual void AddFileChangedListener(const playstate::regex& regex, IFileChangedListener* listener) = 0;
		
		//
		// Adds a event listener for when a specific file is changed
		//
		// @param path The path to the file we want to listen for
		// @param listener
		virtual void AddFileChangedListener(const playstate::string& path, IFileChangedListener* listener) = 0;
		
		//
		// Removes an event listener for when a specific file is changed
		//
		// @param listener
		virtual void RemoveFileChangedListener(IFileChangedListener* listener) = 0;
	};
}
