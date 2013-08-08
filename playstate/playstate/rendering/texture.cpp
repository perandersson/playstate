#include "../memory/memory.h"
#include "texture.h"
#include "state/state_policy.h"
#include "../uuid.h"
using namespace playstate;

Texture::Texture(TextureFormat::Enum format)
	: mTextureId(0), mUUID(UUID::To32Bit()), mFormat(format)
{
}

Texture::~Texture()
{
	if(mTextureId != 0) {
		StatePolicy::InvalidateTexture(mTextureId);
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}
}

void Texture::BindToActiveTexture(uint32 activeTexture, GLenum type)
{
	StatePolicy::SetActiveTexture(activeTexture);
	StatePolicy::BindTexture(type, mTextureId);
}

TextureFormat::Enum Texture::GetFormat() const
{
	return mFormat;
}

uint32 Texture::GetUUID() const
{
	return mUUID;
}

