#include "../../memory/memory.h"
#include "openal_sound_effect.h"
using namespace playstate;

OpenALSoundEffect::OpenALSoundEffect(SoundFormat::Enum format, float32 duration, ALuint bufferID)
	: SoundEffect(format, duration), mBufferID(bufferID)
{
	assert(bufferID != 0 && "Invalid buffer ID. This should not happen");
}

OpenALSoundEffect::~OpenALSoundEffect()
{
	alDeleteBuffers(1, &mBufferID);
	mBufferID = 0;
}
