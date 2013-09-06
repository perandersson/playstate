#pragma once
#include "../../script/script_system.h"

namespace playstate
{
	class GfxProgram;
	class IRenderSystem;
	class GfxProgramFactory
	{
	public:
		GfxProgramFactory(IRenderSystem& renderSystem, ScriptSystem& scriptSystem);
		~GfxProgramFactory();

		//
		// Creates a new gfx program based on the supplied file
		GfxProgram* Create(const playstate::string& fileName);

	private:
		IRenderSystem& mRenderSystem;
		ScriptSystem& mScriptSystem;
	};
}

