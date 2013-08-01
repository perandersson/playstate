#pragma once

#include "default_gfx_program_component.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	class IntGfxProgramComponent : public DefaultGfxProgramComponent
	{
	public:
		IntGfxProgramComponent(GfxProgram& program, GLint componentId, GLenum type);
		virtual ~IntGfxProgramComponent();

	// IGfxProgramComponent
	public:
		virtual void Apply();
		virtual void SetInt(int32 a);
		virtual void SetInt(int32 a, int32 b);
		virtual void SetInt(int32 a, int32 b, int32 c);
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d);
		virtual void MarkAsDirty();

	private:
		GfxProgram& mProgram;
		GLint mComponentId;
		GLenum mType;
		
		int32 mCount;
		int32 mValues[4];
		int32 mValuesSet[4];
	};

}
