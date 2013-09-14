#pragma once
#include <playstate/resources/resource_loader.h>
#include <playstate/filesystem/file_system.h>
#include "openal_sound_engine.h"

namespace playstate
{
	class OpenALSoundEffectResourceLoader : public IResourceLoader
	{
	public:
		OpenALSoundEffectResourceLoader(IFileSystem& fileSystem);
		~OpenALSoundEffectResourceLoader();

	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		IFileSystem& mFileSystem;

		ResourceObject* mDefaultResource;
	};
}

