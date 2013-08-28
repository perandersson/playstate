#pragma once
#include "../music.h"
#include "../../filesystem/file.h"
#include "../exception/sound_exception.h"
#include "../../linked_list.h"

#include <AL/al.h>

namespace playstate
{
	class IOpenALMusicStream
	{
	public:
		virtual ~IOpenALMusicStream() {}

	public:
		//
		// Streams music from this instance and put the result in the supplied OpenAL buffer
		//
		// @param buffer The target buffer
		// @param loop If the stream should be looping
		// @return TRUE if more data can be streamed from the supplied data; FALSE otherwise.
		virtual bool Stream(ALuint buffer, bool loop) = 0;

		//
		// Resets the stream
		virtual void Reset() = 0;
	};

	//
	// 
	class OpenALMusic : public Music
	{
	public:
		LinkedListLink<OpenALMusic> MusicLink;

	public:
		OpenALMusic(float32 duration, auto_array<ALuint> buffers, IOpenALMusicStream* stream);
		virtual ~OpenALMusic();

		//
		// @return TRUE if this music instance is being played already.
		inline bool IsPlaying() const {
			return MusicLink.IsLinked();
		}

		//
		// Attaches this music resource to a source
		//
		void AttachToSource(ALuint source, bool looping);

		//
		// Detaches this music instance from it's attached source
		//
		void DetachFromSource();

		//
		// Update the buffers with new music data
		//
		// @return TRUE if more music are available for processing.
		// @throws playstate::SoundException If an OpenAL error is raised while updating the queued buffers
		bool UpdateStream();
		
	private:
		ALuint mAttachSourceID;
		bool mLooping;
		auto_array<ALuint> mBuffers;
		IOpenALMusicStream* mStream;
	};
}
