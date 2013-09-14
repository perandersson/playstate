#include "../memory/memory.h"
#include "image_resource_loader.h"
#include "../script/luam.h"
#include "../resources/resource_manager.h"
#include <FreeImage.h>
using namespace playstate;

ImageResourceLoader::ImageResourceLoader(IRenderSystem& renderSystem, IFileSystem& fileSystem) : 
	mRenderSystem(renderSystem), mFileSystem(fileSystem), mDefaultResource(NULL)
{
}

ImageResourceLoader::~ImageResourceLoader()
{
	if(mDefaultResource != NULL) {
		delete mDefaultResource;
		mDefaultResource = NULL;
	}
}

ResourceObject* ImageResourceLoader::Load(IFile& file)
{
	uint32 fileSize = file.Size();
	std::auto_ptr<byte> fileData(new byte[fileSize]);
	file.ReadBytes(fileData.get(), fileSize);

	FIMEMORY* hmem = FreeImage_OpenMemory((BYTE*)fileData.get(), fileSize);
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
	FIBITMAP* dib = NULL;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_LoadFromMemory(fif, hmem, 0);
	if(dib == NULL) {
		return NULL;
	}

	BYTE* bits = FreeImage_GetBits(dib);
	uint32 width = FreeImage_GetWidth(dib);
	uint32 height = FreeImage_GetHeight(dib);
	uint32 bpp = FreeImage_GetBPP(dib);
	if((bits == 0) || (width == 0) || (height == 0)) {
		return NULL;
	}

	TextureFormat::Enum format = TextureFormat::BGR;
	if(bpp == 32) {
		format = TextureFormat::BGRA;
	}

	ITexture2D* texture = NULL;
	try {
		texture = mRenderSystem.CreateTexture2D(Size(width, height), format, bits);
		
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(hmem);
	} catch(RenderingException& e) {
		
		FreeImage_Unload(dib);
		FreeImage_CloseMemory(hmem);

		THROW_EXCEPTION(RenderingException, "Could not load texture: '%s'. Reason: '%s'", 
			file.GetPath(), e.GetMessage().c_str());
	}
		
	return texture;
}

ResourceObject* ImageResourceLoader::GetDefaultResource()
{
	if(mDefaultResource == NULL) {
		std::auto_ptr<IFile> file = mFileSystem.OpenFile("/engine/defaults/texture2d.png");
		assert(file->Exists() && "The default resource must exist");
		mDefaultResource = Load(*file);
	}

	return mDefaultResource;
}

bool ImageResourceLoader::IsThreadable() const
{
	return true;
}
