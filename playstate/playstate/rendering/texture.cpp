#include "../memory/memory.h"
#include "texture.h"
#include "state/state_policy.h"
#include "../uuid.h"
using namespace playstate;

Texture::Texture()
	: mTextureId(0), mUniqueId(UUID::To32Bit()), mTextureFormat(TextureFormat::RGB),
	Format(mTextureFormat), UniqueId(mUniqueId)
{
}

Texture::Texture(TextureFormat::Enum format)
	: mTextureId(0), mUniqueId(UUID::To32Bit()), mTextureFormat(format),
	Format(mTextureFormat), UniqueId(mUniqueId)
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
