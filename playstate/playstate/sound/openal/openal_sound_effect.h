#pragma once
#include "../sound_effect.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

namespace playstate
{
	class OpenALSoundEffect : public SoundEffect
	{
	public:
		OpenALSoundEffect(SoundFormat::Enum format, float32 duration, ALuint bufferID);
		virtual ~OpenALSoundEffect();

		//
		//
		inline ALuint GetBufferID() const {
			return mBufferID;
		}

	private:
		ALuint mBufferID;
	};
}
