#pragma once

#include "default_gfx_program_component.h"
#include <gl/glew.h>

namespace playstate
{
	class OGL3GfxProgram;

	class MatrixGfxProgramComponent : public DefaultGfxProgramComponent
	{
	public:
		MatrixGfxProgramComponent(OGL3GfxProgram& program, GLint componentId, GLenum type);
		virtual ~MatrixGfxProgramComponent();

	// IGfxProgramComponent
	public:
		virtual void Apply();
		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void MarkAsDirty();

	private:
		OGL3GfxProgram& mProgram;
		GLint mComponentId;
		GLenum mType;
		
		float32 mValues[16];
	};

}
