#pragma once
#include "../../resources/resource_loader.h"
#include "openal_sound_engine.h"

namespace playstate
{
	class OpenALResourceLoader : public IResourceLoader
	{
	public:
		OpenALResourceLoader(OpenALSoundEngine& soundEngine);
		virtual ~OpenALResourceLoader();

	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		OpenALSoundEngine& mSoundEngine;
	};

}
