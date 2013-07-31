#include "../memory/memory.h"
#include "texture.h"
#include "state/state_policy.h"
#include "../uuid.h"
using namespace playstate;

Texture::Texture()
	: mTextureId(0), mUniqueId(UUID::To32Bit()), mFormat(TextureFormat::RGB)
{
}

Texture::Texture(TextureFormat::Enum format)
	: mTextureId(0), mUniqueId(UUID::To32Bit()), mFormat(format)
{
}

Texture::~Texture()
{
	if(mTextureId != 0) {
		glDeleteTextures(1, &mTextureId);
		mTextureId = 0;
	}
}

void Texture::BindToActiveTexture(GLenum type)
{
	StatePolicy::BindTexture(type, mTextureId);
}

TextureFormat::Enum Texture::GetFormat() const
{
	return mFormat;
}

uint32 Texture::GetUniqueId() const
{
	return mUniqueId;
}

