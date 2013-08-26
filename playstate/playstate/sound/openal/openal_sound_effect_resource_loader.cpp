#include "../../memory/memory.h"
#include "openal_sound_effect_resource_loader.h"
#include "openal_sound_effect.h"
#include "../../logging/logger.h"
using namespace playstate;

OpenALSoundEffectResourceLoader::OpenALSoundEffectResourceLoader(IFileSystem& fileSystem)
	: mFileSystem(fileSystem), mDefaultResource(NULL)
{
}

OpenALSoundEffectResourceLoader::~OpenALSoundEffectResourceLoader()
{
	delete mDefaultResource;
}

namespace {
	// As seen from the header image found at: https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
	struct WAVHeader
	{
		char ChunkID[4];
		unsigned long ChunkSize;
		char Format[4];
		char Subchunk1ID[4];
		unsigned long Subchunk1Size;
		unsigned short AudioFormat;
		unsigned short NumChannels;
		unsigned long SampleRate;
		unsigned long ByteRate;
		unsigned short BlockAlign;
		unsigned short BitsPerSample;
		char Subchunk2ID[4];
		unsigned long Subchunk2Size;
	};
}

ResourceObject* OpenALSoundEffectResourceLoader::Load(IFile& file)
{
	WAVHeader header;
	if(file.ReadBytes((byte*)&header, sizeof(WAVHeader)) != sizeof(WAVHeader))
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Header was invalid", file.GetPath().c_str());
	
	const char _chunkid[] = {header.ChunkID[0], header.ChunkID[1], header.ChunkID[2], header.ChunkID[3], 0};
	if(strcmp(_chunkid, "RIFF") != 0)
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Header was invalid", file.GetPath().c_str());
	
	const char _format[] = {header.Format[0], header.Format[1], header.Format[2], header.Format[3], 0};
	if(strcmp(_format, "WAVE") != 0)
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Header was invalid", file.GetPath().c_str());
	
	const char _subchunk1id[] = {header.Subchunk1ID[0], header.Subchunk1ID[1], header.Subchunk1ID[2], header.Subchunk1ID[3], 0};
	if(strcmp(_subchunk1id, "fmt ") != 0)
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Header was invalid", file.GetPath().c_str());
	
	const char _subchunk2id[] = {header.Subchunk2ID[0], header.Subchunk2ID[1], header.Subchunk2ID[2], header.Subchunk2ID[3], 0};
	if(strcmp(_subchunk2id, "data") != 0)
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Header was invalid", file.GetPath().c_str());

	auto_array<byte> data(header.Subchunk2Size);
	if(file.ReadBytes(data.Get(), sizeof(WAVHeader), header.Subchunk2Size) != header.Subchunk2Size)
		THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Data size was invalid", file.GetPath().c_str());
	
	ALuint bufferID = 0;
	alGenBuffers(1, &bufferID);
	
	ALenum alFormat;
	SoundFormat::Enum format = SoundFormat::Default;
	if(header.BitsPerSample == 8) {
		if(header.NumChannels == 1) {
			format = SoundFormat::MONO8;
			alFormat = AL_FORMAT_MONO8;
		} else if(header.NumChannels == 2) {
			format = SoundFormat::STEREO8;
			alFormat = AL_FORMAT_STEREO8;
		}
	} else if(header.BitsPerSample == 16) {
		if(header.NumChannels == 1) {
			format = SoundFormat::MONO16;
			alFormat = AL_FORMAT_MONO16;
		} else if(header.NumChannels == 2) {
			format = SoundFormat::STEREO16;
			alFormat = AL_FORMAT_STEREO16;
		}
	}

	if(format != SoundFormat::MONO8 && format != SoundFormat::MONO16) {
		ILogger::Get().Warn("The loaded sound effect: '%s' is not a MONO sound. 3D positioning will not work",
			file.GetPath().c_str());
	}

	const float32 duration = header.SampleRate / (header.BitsPerSample / 8.0f) / (header.SampleRate * 1.0f);
	alBufferData(bufferID, alFormat, data.Get(), data.Length(), header.SampleRate);
	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		THROW_EXCEPTION(LoadResourceException, "Could not load sound buffer with data for sound: '%s'", file.GetPath().c_str());
	
	return new OpenALSoundEffect(format, duration, bufferID);
}

ResourceObject* OpenALSoundEffectResourceLoader::GetDefaultResource()
{
	if(mDefaultResource == NULL) {
		std::auto_ptr<IFile> file = mFileSystem.OpenFile("/engine/defaults/sound_effect.wav");
		assert(file->Exists() && "The default resource does not exist");
		mDefaultResource = Load(*file);
	}
	return mDefaultResource;
}

bool OpenALSoundEffectResourceLoader::IsThreadable() const
{
	return true;
}