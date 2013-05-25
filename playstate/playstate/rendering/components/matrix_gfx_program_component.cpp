#include "../../memory/memory.h"
#include "matrix_gfx_program_component.h"
#include "../gfx_program.h"

#include <cassert>

using namespace playstate;

MatrixGfxProgramComponent::MatrixGfxProgramComponent(GfxProgram& program, GLint componentId, GLenum type)
	: mProgram(program), mComponentId(componentId), mType(type)
{
}

MatrixGfxProgramComponent::~MatrixGfxProgramComponent()
{
}

void MatrixGfxProgramComponent::Apply()
{
	glUniformMatrix4fv(mComponentId, 1, false, mValues);

	CHECK_GL_ERROR();
}

void MatrixGfxProgramComponent::SetMatrix(const Matrix4x4& matrix)
{
	assert(mType == GL_FLOAT_MAT4 && "Invalid matrix type type");
	memcpy(mValues, matrix.Values, sizeof(float32) * 16);

	if(mProgram.Applied)
		MatrixGfxProgramComponent::Apply();
}
