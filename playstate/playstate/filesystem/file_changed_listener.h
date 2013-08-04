#pragma once

namespace playstate
{
	class IFile;

	class FileChangeAction
	{
	public:
		enum Enum {
			ADDED,
			DELETED,
			MODIFIED
		};
	};

	class IFileChangedListener
	{
	public:
		virtual ~IFileChangedListener() {}

	public:
		//
		// @param file The file that was changed
		virtual void FileChanged(const IFile& file, FileChangeAction::Enum action) = 0;
	};
}

