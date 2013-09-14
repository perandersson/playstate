#pragma once

#include "../../types.h"

namespace playstate
{
	class OGL3GfxProgram;
	class IRenderSystem;
	class ScriptSystem;
	class GfxProgramFactory
	{
	public:
		GfxProgramFactory(IRenderSystem& renderSystem, ScriptSystem& scriptSystem);
		~GfxProgramFactory();

		//
		// Creates a new gfx program based on the supplied file
		OGL3GfxProgram* Create(const playstate::string& fileName);

	private:
		IRenderSystem& mRenderSystem;
		ScriptSystem& mScriptSystem;
	};
}

