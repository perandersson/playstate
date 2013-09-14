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
	
	GLuint textureId = 0;	
	glGenTextures(1, &textureId);
	StatePolicy::BindTexture(GL_TEXTURE_2D, textureId);
	GLint format = GL_BGR;
	GLint internalFormat = GL_RGB;
	GLint components = 3;
	if(bpp == 32) {
		format = GL_BGRA;
		internalFormat = GL_RGBA;
		components = 4;
	}
	
	// TODO Add support for 8-bit textures.
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, bits);

	FreeImage_Unload(dib);
	FreeImage_CloseMemory(hmem);

	glFlush();

	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not load texture: '%s'. Reason: %d", file.GetPath(), error);
	}
	
	TextureFormat::Enum textureFormat = TextureFormat::RGB;
	if(internalFormat == GL_RGBA) {
		textureFormat = TextureFormat::RGBA;
	}
	
	return new Texture2D(textureId, Size(width, height), textureFormat);
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

int playstate::Texture2D_Load(lua_State* L)
{
	playstate::string path = lua_tostring(L, -1); lua_pop(L, 1);
	Resource<Texture2D> texture = IResourceManager::Get().GetResource<Texture2D>(path);
	luaM_pushresource(L, texture.GetResourceData());
	return 1;
}
