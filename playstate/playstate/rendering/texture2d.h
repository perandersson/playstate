#pragma once

#include "../resources/resource.h"
#include "../resources/resource_loader.h"
#include "../types.h"
#include "state/state_policy.h"
#include <gl/glew.h>

namespace playstate
{
	enum TextureFormat
	{
		RGB, 
		RGBA,
		RGBA8,
		RGBA12,
		RGBA16,
		RGB10_A2,
		RGBA16F,
		DEPTH24,
		DEPTH24_STENCIL8,
		UNKOWN_TEXTURE_FORMAT,
	};

	class RenderSystem;
	class IFileSystem;

	//
	// 
	class Texture2D : public ResourceObject
	{
	public:
		Texture2D(GLuint textureId, uint32 width, uint32 height, TextureFormat format);
		virtual ~Texture2D();
		
		void Bind(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt);

	public:
		const uint32& Width;
		const uint32& Height;
		const GLuint& TextureID;
		const TextureFormat& Format;
		const uint32& StateID;
	
	private:
		uint32 mWidth;
		uint32 mHeight;
		TextureFormat mFormat;
		GLuint mTextureId;
		uint32 mStateId;
		
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

