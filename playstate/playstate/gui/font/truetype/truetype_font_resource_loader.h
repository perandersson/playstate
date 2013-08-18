#pragma once

#include "../../../resources/resource_loader.h"
#include "../../../filesystem/file_system.h"

namespace playstate
{
	class TrueTypeFontResourceLoader : public IResourceLoader
	{
	public:
		TrueTypeFontResourceLoader(IFileSystem& fileSystem);
		virtual ~TrueTypeFontResourceLoader();
	
	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		int GetNextP2(int a) const;

	private:
		IFileSystem& mFileSystem;
		ResourceObject* mDefaultResource;
	};
}
