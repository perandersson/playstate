#pragma once

#include "texture.h"
#include "../resources/resource_loader.h"
#include "state/state_policy.h"
#include "../math/point.h"

extern "C"
{
   #include <lua.h>
   #include <lauxlib.h>
   #include <lualib.h>
}

namespace playstate
{
	class IFileSystem;
	
	//
	// 
	class Texture2D : public ITexture
	{
	public:
		Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format);
		virtual ~Texture2D();
		
		//
		// @return This textures width in pixels
		inline const Size& GetSize() const {
			return mSize;
		}

		//
		// Binds this texture to the supplied active texture block
		//
		// @param activeTexture
		void Bind(uint32 activeTexture);

		//
		// Update this textures filters
		//
		//
		void UpdateFilters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt);
		
		//
		// @return A unique identifier with for this texture. This is needed because the
		//	ID object ID's generated by OpenGL are reused between deletes. The same thing is done by the
		//	operating system as well (regarding memory allocations and deletions).
		inline uint32 GetUUID() const {
			return mUUID;
		}

		inline GLuint GetID() const {
			return mTextureID;
		}

	// ITexture
	public:
		virtual TextureFormat::Enum GetFormat() const;

	protected:
		GLuint mTextureID;
		uint32 mUUID;
		TextureFormat::Enum mFormat;

		Size mSize;
		
		MinFilter::Enum mMinFilter;
		MagFilter::Enum mMagFilter;
		TextureWrap::Enum mWS;
		TextureWrap::Enum mWT;
	};

	class IRenderSystem;

	//
	// 
	class Texture2DResourceLoader : public IResourceLoader
	{
	public:
		Texture2DResourceLoader(IRenderSystem& renderSystem, IFileSystem& fileSystem);
		~Texture2DResourceLoader();
		
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

	
	//
	// Script integration
	//

	extern int Texture2D_Load(lua_State* L);
	static luaL_Reg Texture2D_Methods[] = {
		{ "Load", Texture2D_Load },
		{ NULL, NULL }
	};
}

