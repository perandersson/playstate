#include "../memory/memory.h"
#include "texture2d.h"
#include "state/state_policy.h"
using namespace playstate;

Texture2D::Texture2D(GLuint textureID, const Size& size, TextureFormat::Enum format) 
	: mTextureID(textureID), mUUID(UUID::To32Bit()), mFormat(format), mSize(size),
	mMinFilter(MinFilter::UNKNOWN), mMagFilter(MagFilter::UNKNOWN), mWS(TextureWrap::UNKNOWN), mWT(TextureWrap::UNKNOWN)
{
}

Texture2D::~Texture2D()
{
	if(mTextureID != 0) {
		StatePolicyGuard::InvalidateTexture(mTextureID);
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
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

TextureFormat::Enum Texture2D::GetFormat() const
{
	return mFormat;
}
