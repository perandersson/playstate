#include <playstate/memory/memory.h>
#include "ogl3_texture2d.h"
#include "ogl3_state_policy.h"
using namespace playstate;

OGL3Texture2D::OGL3Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format) 
	: mTextureID(textureID), mUUID(UUID::To32Bit()), mFormat(format), mSize(size),
	mMinFilter(MinFilter::Default), mMagFilter(MagFilter::Default), mWS(TextureWrap::Default), mWT(TextureWrap::Default)
{
}

OGL3Texture2D::~OGL3Texture2D()
{
	if(mTextureID != 0) {
		OGL3StatePolicyGuard::InvalidateTexture(mTextureID);
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
}

void OGL3Texture2D::Bind(uint32 activeTexture)
{
	OGL3StatePolicyGuard::SetActiveTexture(activeTexture);
	OGL3StatePolicyGuard::BindTexture(GL_TEXTURE_2D, mTextureID);
}

void OGL3Texture2D::UpdateFilters(MinFilter::Enum minFilter, MagFilter::Enum magFilter, TextureWrap::Enum ws, TextureWrap::Enum wt)
{
	if(mMinFilter != minFilter) {
		mMinFilter = minFilter;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, OGL3MinFilter[mMinFilter]);
	}

	if(mMagFilter != magFilter) {
		mMagFilter = magFilter;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, OGL3MagFilter[mMagFilter]);
	}

	if(mWS != ws) {
		mWS = ws;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGL3TextureWrap[mWS]);
	}

	if(mWT != wt) {
		mWT = wt;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGL3TextureWrap[mWT]);
	}
}

TextureFormat::Enum OGL3Texture2D::GetFormat() const
{
	return mFormat;
}

const Size& OGL3Texture2D::GetSize() const
{
	return mSize;
}