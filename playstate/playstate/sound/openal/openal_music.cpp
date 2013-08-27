#include "../../memory/memory.h"
#include "openal_music.h"
using namespace playstate;

OpenALMusic::OpenALMusic(float32 duration, auto_array<ALuint> buffers, IOpenALMusicStream* stream)
	: Music(duration), mBuffers(buffers), mStream(stream)
{
	assert_not_null(stream);
}

OpenALMusic::~OpenALMusic()
{
	alDeleteBuffers(mBuffers.Length(), mBuffers.Get());
	delete mStream;
}

void OpenALMusic::AttachToSource(ALuint source, bool looping)
{
	assert(looping == true && "Non-looping music is not working at the moment");

	mAttachSourceID = source;
	mLooping = looping;
}

void OpenALMusic::DetachFromSource()
{
	alSourceStop(mAttachSourceID);
	mAttachSourceID = 0;
	mLooping = false;
	MusicLink.Unlink();
}

bool OpenALMusic::Update()
{
	int processed = 0;
	bool active = true;

	int32 buffersProcessed;
	alGetSourcei(mAttachSourceID, AL_BUFFERS_PROCESSED, &buffersProcessed);
	
	for(int32 i = 0; i < buffersProcessed && active; ++i) {
		ALuint buffer;
		alSourceUnqueueBuffers(mAttachSourceID, 1, &buffer);
		if(alGetError() != AL_NO_ERROR) {
			THROW_EXCEPTION(SoundException, "Could not unqueue music buffer from source");
		}

		active = mStream->Stream(buffer);
		if(!active && mLooping) {
			mStream->Reset();
			active = true;
		}

		alSourceQueueBuffers(mAttachSourceID, 1, &buffer); 
		if(alGetError() != AL_NO_ERROR) {
			THROW_EXCEPTION(SoundException, "Could not queue music buffer to source");
		}
	}

	return active;
}
