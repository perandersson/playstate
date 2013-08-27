#pragma once
#include "../sound_engine.h"
#include "../../linked_list.h"
#include "openal_music.h"
#include "openal_sound_effect.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace playstate
{
	// The maximum number of sources the engine tries to allocate
	static const uint32 MaxMusicSources = 3U;
	static const uint32 MaxSoundSources = 29U;
	static const uint32 MaxSources = MaxMusicSources + MaxSoundSources;

	class OpenALSoundEngine : public ISoundEngine, public ISoundListener
	{
	public:
		OpenALSoundEngine();
		virtual ~OpenALSoundEngine();

		//
		// Update this sound engines sound streams
		void UpdateStreams();

	// ISoundEngine
	public:
		virtual void Play(SoundEffect* effect);
		virtual void Play(SoundEffect* effect, const Vector3& position);
		virtual void Play(Music* music);
		virtual void Play(Music* music, float32 fadeInTime);
		virtual void Stop(Music* music);
		virtual void Stop(Music* music, float32 fadeOutTime);
		virtual void SetMasterVolume(float32 volume);
		virtual void SetMusicVolume(float32 volume);
		virtual void SetSoundEffectVolume(float32 volume);
		virtual ISoundListener* GetListener();

	// ISoundListener
	public:
		virtual void SetPosition(const Vector3& position);
		virtual void LookAt(const Vector3& direction, const Vector3& up);

	private:
		//
		// @return The next source in the sources round-robin list. 
		ALuint FindNextSource();

		ALuint FindNextMusicSource();

		void PlaySoundEffectBuffer(ALuint sourceID, ALuint bufferID);

	private:
		ALuint mMusicSources[MaxMusicSources];
		uint32 mNextMusicSourceIndex;
		LinkedList<OpenALMusic> mMusicResources;

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
