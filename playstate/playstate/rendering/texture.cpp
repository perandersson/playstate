#include "../memory/memory.h"
#include "texture.h"
#include "state/state_policy.h"
#include "../uuid.h"
using namespace playstate;

Texture::Texture(GLuint id, TextureFormat::Enum format)
	: mTextureID(id), mUUID(UUID::To32Bit()), mFormat(format)
{
}

Texture::~Texture()
{
	if(mTextureID != 0) {
		StatePolicy::InvalidateTexture(mTextureID);
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}
}

