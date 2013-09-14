#pragma once
#include "../resources/resource_loader.h"
#include "../filesystem/file_system.h"
#include "../rendering/render_system.h"

namespace playstate
{
	//
	// 
	class ImageResourceLoader : public IResourceLoader
	{
	public:
		ImageResourceLoader(IRenderSystem& renderSystem, IFileSystem& fileSystem);
		virtual ~ImageResourceLoader();
		
	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;
		
	private:
		IRenderSystem& mRenderSystem;
		IFileSystem& mFileSystem;

		ResourceObject* mDefaultResource;
	};
}
