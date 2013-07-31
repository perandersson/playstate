#include "../../memory/memory.h"
#include "float_gfx_program_component.h"
#include "../gfx_program.h"

#include <cassert>

using namespace playstate;

FloatGfxProgramComponent::FloatGfxProgramComponent(GfxProgram& program, GLint componentId, GLenum type)
	: mProgram(program), mComponentId(componentId), mType(type), mCount(0)
{
	mValues[0] = 0.0f;
	mValues[1] = 0.0f;
	mValues[2] = 0.0f;
	mValues[3] = 0.0f;
	
	mValuesSet[0] = FLT_MAX;
	mValuesSet[1] = FLT_MAX;
	mValuesSet[2] = FLT_MAX;
	mValuesSet[3] = FLT_MAX;
}

FloatGfxProgramComponent::~FloatGfxProgramComponent()
{
}

void FloatGfxProgramComponent::Apply()
{
	switch(mCount) {
	case 3:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		glUniform3fv(mComponentId, 1, mValues);
		break;
	case 2:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform2fv(mComponentId, 1, mValues);
		break;
	case 4:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1] &&
			mValuesSet[2] == mValues[2] &&
			mValuesSet[3] == mValues[3])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		mValuesSet[2] = mValues[2];
		mValuesSet[3] = mValues[3];
		glUniform4fv(mComponentId, 1, mValues);
		break;
	case 1:
		if(mValuesSet[0] == mValues[0])
			return;
		mValuesSet[0] = mValues[0];
		glUniform1fv(mComponentId, 1, mValues);
		break;
	default:
		return;
	}

	CHECK_GL_ERROR();
}

void FloatGfxProgramComponent::SetFloat(float32 a)
{
	mCount = 1;
	mValues[0] = a;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetFloat(float32 a, float32 b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetFloat(float32 a, float32 b, float32 c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetFloat(float32 a, float32 b, float32 c, float32 d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetColorRGBA(const Color& color)
{
	mCount = 4;
	mValues[0] = color.Red;
	mValues[1] = color.Green;
	mValues[2] = color.Blue;
	mValues[3] = color.Alpha;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetColorRGB(const Color& color)
{
	mCount = 3;
	mValues[0] = color.Red;
	mValues[1] = color.Green;
	mValues[2] = color.Blue;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetVector3(const Vector3& vec)
{
	mCount = 3;
	mValues[0] = vec.X;
	mValues[1] = vec.Y;
	mValues[2] = vec.Z;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}

void FloatGfxProgramComponent::SetVector2(const Vector2& vec)
{
	mCount = 2;
	mValues[0] = vec.X;
	mValues[1] = vec.Y;

	if(mProgram.IsApplied())
		FloatGfxProgramComponent::Apply();
}
