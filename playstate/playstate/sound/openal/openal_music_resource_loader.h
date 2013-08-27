#pragma once
#include "../../resources/resource_loader.h"
#include "openal_sound_engine.h"

namespace playstate
{
	class OpenALMusicResourceLoader : public IResourceLoader
	{
	public:
		OpenALMusicResourceLoader(OpenALSoundEngine& soundEngine);
		virtual ~OpenALMusicResourceLoader();

	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		OpenALSoundEngine& mSoundEngine;
	};

}
