#pragma once

#include "../types.h"

#include <memory>
#include <sstream>

namespace playstate
{
	class IDirectory;

	class IFile
	{
	public:
		virtual ~IFile() {}

		//
		// @return A string containing the entire file.
		virtual std::istringstream Read() const = 0;

		//
		// @return A new file based on the current files path. This method always returns an object.
		virtual std::auto_ptr<IFile> OpenFile(const playstate::string& path) const = 0;

		//
		// @return the size of the current file.
		virtual uint64 Size() const = 0;
		
		//
		// @return The number of bytes that where read
		virtual uint64 ReadBytes(byte* buffer, uint64 offset, uint64 bufferSize) const = 0;

		//
		// @return
		virtual uint64 ReadBytes(byte* buffer, uint64 bufferSize) const = 0;

		//
		// @return The path to the file
		virtual const playstate::string& GetPath() const = 0;
		
		//
		// @return A new directory instance where this file is located.
		virtual std::auto_ptr<IDirectory> GetDirectory() const = 0;

		//
		// @return TRUE if the file exists; FALSE otherwise
		virtual bool Exists() const = 0;
	};
}
