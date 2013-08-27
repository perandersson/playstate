#include "../../memory/memory.h"
#include "openal_music_resource_loader.h"
#include "openal_music.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#include <ogg/ogg.h>

using namespace playstate;

OpenALResourceLoader::OpenALResourceLoader(OpenALSoundEngine& soundEngine)
	: mSoundEngine(soundEngine)
{
}

OpenALResourceLoader::~OpenALResourceLoader()
{
}

class OggVorbisMusicStream : public IOpenALMusicStream
{
	static const uint32 BufferSizeInBytes = 1024 * 16; // 16kb

public:
	OggVorbisMusicStream(std::auto_ptr<IFile> file) : mFile(file) {}
	virtual ~OggVorbisMusicStream () {}

// IOpenALMusicStream
public:
	virtual bool Stream(ALuint buffer) {
		
		return true;
	}

	virtual void Reset() {
		mOffset = 0;
	}

	static size_t AR_readOgg(void* dest, size_t size1, size_t size2, void* fh) {

	}

	static int AR_seekOgg(void* fh, ogg_int64_t to, int type) {
		OggVorbisMusicStream* stream = reinterpret_cast<OggVorbisMusicStream*>(fh);
		switch(type) {
		case SEEK_CUR:
			stream->mCurrentPtr += to;
			break;
		case SEEK_END:
			stream->mCurrentPtr = stream->mFilePtr + stream->mFileSize - 0;
			break;
		case SEEK_SET:
			stream->mCurrentPtr = stream->mFilePtr + to;
			break;
		}


	}

	static int AR_closeOgg(void* fh) {
		return 0;
	}

	static long AR_tellOgg(void* fh) {
		OggVorbisMusicStream* stream = reinterpret_cast<OggVorbisMusicStream*>(fh);
		return (long)(stream->mCurrenPtr - stream->mFilePtr);
	}

private:
	std::auto_ptr<IFile> mFile;
	uint32 mOffset;
	byte* mCurrentPtr;
	byte* mFilePtr;
	uint32 mFileSize;
};

ResourceObject* OpenALResourceLoader::Load(IFile& file)
{
	OggVorbis_File* ov = new OggVorbis_File;
	ov_callbacks callbacks;
	ogg_file t;
	t.


	OggVorbisMusicStream* stream = new OggVorbisMusicStream(file.OpenFile(file.GetPath()));

	return new Music();
}

ResourceObject* OpenALResourceLoader::GetDefaultResource()
{
	return NULL;
}

bool OpenALResourceLoader::IsThreadable() const
{
	return false;
}
