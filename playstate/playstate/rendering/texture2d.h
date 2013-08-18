#pragma once

#include "texture.h"
#include "../resources/resource_loader.h"
#include "state/state_policy.h"

namespace playstate
{
	class RenderSystem;
	class IFileSystem;

	//
	// 
	class Texture2D : public Texture
	{
	public:
		Texture2D(GLuint textureId, uint32 width, uint32 height, TextureFormat::Enum format);
		virtual ~Texture2D();
		
		void Bind(uint32 activeTexture, MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt);

		//
		// @return This textures width in pixels
		uint32 GetWidth() const;

		//
		// @return This textures height in pixels
		uint32 GetHeight() const;

		//void Update(

	protected:
		uint32 mWidth;
		uint32 mHeight;
		
		MinFilter::Enum mMinFilter;
		MagFilter::Enum mMagFilter;
		TextureWrap::Enum mWS;
		TextureWrap::Enum mWT;
	};

	//
	// 
	class Texture2DResourceLoader : public IResourceLoader
	{
	public:
		Texture2DResourceLoader(RenderSystem& renderSystem, IFileSystem& fileSystem);
		~Texture2DResourceLoader();
		
	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;
		
	private:
		RenderSystem& mRenderSystem;
		IFileSystem& mFileSystem;

		ResourceObject* mDefaultResource;
	};
}

