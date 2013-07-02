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
		
		void Bind(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt);

	public:
		const uint32& Width;
		const uint32& Height;

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

