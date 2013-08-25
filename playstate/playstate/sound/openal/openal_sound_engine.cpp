#include "../../memory/memory.h"
#include "openal_sound_engine.h"
#include "../music.h"
#include "../sound_effect.h"
using namespace playstate;

OpenALSoundEngine::OpenALSoundEngine()
	: mNumSources(0), mNextSourceIndex(0), mMasterVolume(1.0f), mMusicVolume(0.5f), mSoundEffectVolume(0.5f),
	mDevice(NULL), mContext(NULL)
{
	mDevice = alcOpenDevice(NULL);
	mContext = alcCreateContext(mDevice, NULL);
	alcMakeContextCurrent(mContext);

	// Try to generate as many sources as possible
	memset(mMusicSources, 0, sizeof(mMusicSources));
	alGenSources(MaxMusicSources, mMusicSources);

	memset(mSources, 0, sizeof(mSources));
	for(uint32 i = 0; i < MaxSoundSources; ++i) {
		alGenSources(1, &mSources[i]);
		mNumSources++;
	}
}

OpenALSoundEngine::~OpenALSoundEngine()
{
	alDeleteSources(MaxMusicSources, mMusicSources);
	alDeleteSources(mNumSources, mSources);

	alcMakeContextCurrent(NULL);
    alcDestroyContext(mContext);
	mContext = NULL;
    alcCloseDevice(mDevice);
	mDevice = NULL;
}

void OpenALSoundEngine::Play(Music* music)
{
	assert_not_null(music);
	const ALuint source = 0; //<-- fix this

	// Set position at a relative position to the player so that
	// the sound has a "non-positioned" sound
	alSource3i(source, AL_POSITION, 0, 0, -1);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(source, AL_ROLLOFF_FACTOR, 0);

}

void OpenALSoundEngine::Stop(Music* music)
{
}

void OpenALSoundEngine::Play(SoundEffect* effect)
{
	assert_not_null(effect);
	const ALuint source = FindNextSource();

	alSource3i(source, AL_POSITION, 0, 0, -1);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
	PlaySoundEffectBuffer(source, effect->GetBufferID());
}

void OpenALSoundEngine::Play(SoundEffect* effect, const Vector3& position)
{
	assert_not_null(effect);
	const ALuint source = FindNextSource();

	alSourcefv(source, AL_POSITION, position.Points);
	alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
	PlaySoundEffectBuffer(source, effect->GetBufferID());
}

void OpenALSoundEngine::PlaySoundEffectBuffer(ALuint sourceID, ALuint bufferID)
{
	alSourcef(sourceID, AL_GAIN, mSoundEffectVolume * mMasterVolume);
	alSourcei(sourceID, AL_BUFFER, bufferID);
    alSourcei(sourceID, AL_LOOPING, AL_FALSE);
	alSourcePlay(sourceID);
    assert(alGetError() == AL_NO_ERROR && "Could not set source parameters and play buffer");
}

void OpenALSoundEngine::SetMasterVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;

	mMasterVolume = volume;

	// Update all music sources with the new volume
	for(uint32 i = 0; i < MaxMusicSources; ++i) {
		alSourcef(mMusicSources[i], AL_GAIN, mMusicVolume * mMasterVolume);
	}
}

void OpenALSoundEngine::SetMusicVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;
	
	mMusicVolume = volume;
	
	// Update all existing music instances with the new volume
	for(uint32 i = 0; i < MaxMusicSources; ++i) {
		alSourcef(mMusicSources[i], AL_GAIN, mMusicVolume * mMasterVolume);
	}
}

void OpenALSoundEngine::SetSoundEffectVolume(float32 volume)
{
	volume = volume > 1.0f ? 1.0f : volume;
	volume = volume < 0.0f ? 0.0f : volume;

	mSoundEffectVolume = volume;
}

ISoundListener* OpenALSoundEngine::GetListener()
{
	return this;
}

void OpenALSoundEngine::SetPosition(const Vector3& position)
{
	static const float32 velocity[] = {0.0f, 0.0f, 0.0f};
	static const float32 orientation[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};

	alListenerfv(AL_POSITION, position.Points);
	alListenerfv(AL_VELOCITY, velocity);
	alListenerfv(AL_ORIENTATION, orientation);
}

ALuint OpenALSoundEngine::FindNextSource()
{
	ALuint index = mSources[mNextSourceIndex++];
	if(mNextSourceIndex >= mNumSources)
		mNextSourceIndex = 0;
	return index;
}
