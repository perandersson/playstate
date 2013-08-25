#pragma once
#include "../sound_engine.h"
#include "../../linked_list.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace playstate
{
	class Music;
	class SoundEffect;

	// The maximum number of sources the engine tries to allocate
	static const uint32 MaxMusicSources = 6U;
	static const uint32 MaxSoundSources = 26U;
	static const uint32 MaxSources = MaxMusicSources + MaxSoundSources;

	class OpenALSoundEngine : public ISoundEngine, public ISoundListener
	{
	public:
		OpenALSoundEngine();
		virtual ~OpenALSoundEngine();

		//
		// Starts playing music
		void Play(Music* music);

		void Stop(Music* music);

	// ISoundEngine
	public:
		virtual void Play(SoundEffect* effect);
		virtual void Play(SoundEffect* effect, const Vector3& position);
		virtual void SetMasterVolume(float32 volume);
		virtual void SetMusicVolume(float32 volume);
		virtual void SetSoundEffectVolume(float32 volume);
		virtual ISoundListener* GetListener();

	// ISoundListener
	public:
		virtual void SetPosition(const Vector3& position);

	private:
		//
		// @return The next source in the sources round-robin list. 
		ALuint FindNextSource();

		void PlaySoundEffectBuffer(ALuint sourceID, ALuint bufferID);

	private:
		ALuint mMusicSources[MaxMusicSources];

		ALuint mSources[MaxSoundSources];
		uint32 mNumSources;
		uint32 mNextSourceIndex;

		float32 mMasterVolume;
		float32 mMusicVolume;
		float32 mSoundEffectVolume;

		ALCdevice* mDevice;
		ALCcontext* mContext;
	};
}
