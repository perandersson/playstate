#include "../memory/memory.h"
#include "texture2d.h"
#include "render_system.h"
#include "../filesystem/file_system.h"
#include "../resources/resource_manager.h"
#include <FreeImage.h>
using namespace playstate;

Texture2D::Texture2D(GLuint textureId, const Size& size, TextureFormat::Enum format) 
	: Texture(textureId, format), mSize(size),
	mMinFilter(MinFilter::UNKNOWN), mMagFilter(MagFilter::UNKNOWN), mWS(TextureWrap::UNKNOWN), mWT(TextureWrap::UNKNOWN)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind(uint32 activeTexture)
{
	StatePolicyGuard::SetActiveTexture(activeTexture);
	StatePolicyGuard::BindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture2D::UpdateFilters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt)
{
	if(mMinFilter != minFilter) {
		mMinFilter = minFilter;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinFilter);
	}

	if(mMagFilter != magFilter) {
		mMagFilter = magFilter;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagFilter);
	}

	if(mWS != ws) {
		mWS = ws;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWS);
	}

	if(mWT != wt) {
		mWT = wt;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWT);
	}
}

Texture2DResourceLoader::Texture2DResourceLoader(IRenderSystem& renderSystem, IFileSystem& fileSystem) : 
	mRenderSystem(renderSystem), mFileSystem(fileSystem), mDefaultResource(NULL)
{
}

Texture2DResourceLoader::~Texture2DResourceLoader()
{
	if(mDefaultResource != NULL) {
		delete mDefaultResource;
		mDefaultResource = NULL;
	}
}

ResourceObject* Texture2DResourceLoader::Load(IFile& file)
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

ResourceObject* Texture2DResourceLoader::GetDefaultResource()
{
	if(mDefaultResource == NULL) {
		std::auto_ptr<IFile> file = mFileSystem.OpenFile("/engine/defaults/texture2d.png");
		assert(file->Exists() && "The default resource must exist");
		mDefaultResource = Load(*file);
	}

	return mDefaultResource;
}

bool Texture2DResourceLoader::IsThreadable() const
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
