#include "../../memory/memory.h"
#include "matrix_gfx_program_component.h"
#include "../ogl3/ogl3_gfx_program.h"

#include <cassert>

using namespace playstate;

MatrixGfxProgramComponent::MatrixGfxProgramComponent(OGL3GfxProgram& program, GLint componentId, GLenum type)
	: mProgram(program), mComponentId(componentId), mType(type)
{
}

MatrixGfxProgramComponent::~MatrixGfxProgramComponent()
{
}

void MatrixGfxProgramComponent::Apply()
{
	glUniformMatrix4fv(mComponentId, 1, false, mValues);
}

void MatrixGfxProgramComponent::SetMatrix(const Matrix4x4& matrix)
{
	assert(mType == GL_FLOAT_MAT4 && "Invalid matrix type type");
	memcpy(mValues, matrix.Values, sizeof(matrix.Values));

	if(mProgram.IsApplied())
		MatrixGfxProgramComponent::Apply();
}

void MatrixGfxProgramComponent::MarkAsDirty()
{
}
