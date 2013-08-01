#pragma once

#include "default_gfx_program_component.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	class MatrixGfxProgramComponent : public DefaultGfxProgramComponent
	{
	public:
		MatrixGfxProgramComponent(GfxProgram& program, GLint componentId, GLenum type);
		virtual ~MatrixGfxProgramComponent();

	// IGfxProgramComponent
	public:
		virtual void Apply();
		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void MarkAsDirty();

	private:
		GfxProgram& mProgram;
		GLint mComponentId;
		GLenum mType;
		
		float32 mValues[16];
	};

}
