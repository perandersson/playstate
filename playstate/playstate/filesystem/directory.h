#pragma once

#include "../types.h"

#include <memory>
#include <vector>

namespace playstate
{
	class IFile;

	class IDirectory
	{
	public:
		virtual ~IDirectory() {}

		//
		// @return A vector containing the paths of all the files under this directory.
		virtual std::vector<playstate::string> GetFiles() const = 0;

		//
		// @return A vector containing the paths of all the directories under this directory.
		virtual std::vector<playstate::string> GetDirectories() const = 0;
		
		//
		// @return A new file based on the current files path. This method always returns an object.
		virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const = 0;

		//
		// @return A new file based on the current files path; NULL if the file doesn't exist.
		virtual std::auto_ptr<IDirectory> OpenDirectory(const playstate::string& path) const = 0;

		//
		// @return The path to this directory.
		virtual const playstate::string& GetPath() const = 0;

		//
		// @return TRUE if the file exists; FALSE otherwise
		virtual bool Exists() const = 0;
	};

}
