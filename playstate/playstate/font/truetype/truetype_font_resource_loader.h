#pragma once

#include "../../resources/resource_loader.h"
#include "../../filesystem/file_system.h"
#include "../../rendering/render_system.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

namespace playstate
{
	class TrueTypeFontResourceLoader : public IResourceLoader
	{
	public:
		TrueTypeFontResourceLoader(IFileSystem& fileSystem, IRenderSystem& renderSystem);
		virtual ~TrueTypeFontResourceLoader();
	
	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		int GetNextP2(int a) const;
		void CopyToBuffer(uint32 s, uint32 t, uint32 width, uint32 height, playstate::byte* target, const playstate::byte* src);

	private:
		IFileSystem& mFileSystem;
		IRenderSystem& mRenderSystem;
		FT_Library mLibrary;
	};
}
