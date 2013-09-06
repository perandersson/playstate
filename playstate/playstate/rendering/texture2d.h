#pragma once

#include "texture.h"
#include "../resources/resource_loader.h"
#include "state/state_policy.h"

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
	class Texture2D : public Texture
	{
	public:
		Texture2D(GLuint textureId, uint32 width, uint32 height, TextureFormat::Enum format);
		virtual ~Texture2D();
		
		//
		// @return This textures width in pixels
		inline uint32 GetWidth() const {
			return mWidth;
		}

		//
		// @return This textures height in pixels
		inline uint32 GetHeight() const {
			return mHeight;
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

	protected:
		uint32 mWidth;
		uint32 mHeight;
		
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

