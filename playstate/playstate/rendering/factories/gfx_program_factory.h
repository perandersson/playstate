#pragma once
#include "../../script/script_system.h"

namespace playstate
{
	class GfxProgram;
	class RenderSystem;
	class GfxProgramFactory
	{
	public:
		GfxProgramFactory(RenderSystem& renderSystem, ScriptSystem& scriptSystem);
		~GfxProgramFactory();

		//
		// Creates a new gfx program based on the supplied file
		GfxProgram* Create(const std::string& fileName);

	private:
		RenderSystem& mRenderSystem;
		ScriptSystem& mScriptSystem;
	};
}

