#include "../../memory/memory.h"
#include "int_gfx_program_component.h"
#include "../gfx_program.h"

#include <cassert>

using namespace playstate;

IntGfxProgramComponent::IntGfxProgramComponent(GfxProgram& program, GLint componentId, GLenum type)
	: mProgram(program), mComponentId(componentId), mType(type), mCount(1)
{
	mValues[0] = 0;
	mValues[1] = 0;
	mValues[2] = 0;
	mValues[3] = 0;

	mValuesSet[0] = INT_MAX;
	mValuesSet[1] = INT_MAX;
	mValuesSet[2] = INT_MAX;
	mValuesSet[3] = INT_MAX;
}

IntGfxProgramComponent::~IntGfxProgramComponent()
{
}

void IntGfxProgramComponent::Apply()
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
		glUniform3iv(mComponentId, 1, mValues);
		break;
	case 2:
		if(mValuesSet[0] == mValues[0] &&
			mValuesSet[1] == mValues[1])
			return;
		mValuesSet[0] = mValues[0];
		mValuesSet[1] = mValues[1];
		glUniform3iv(mComponentId, 1, mValues);
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
		glUniform3iv(mComponentId, 1, mValues);
		break;
	case 1:
		if(mValuesSet[0] == mValues[0])
			return;
		mValuesSet[0] = mValues[0];
		glUniform3iv(mComponentId, 1, mValues);
		break;
	default:
		return;
	}

	CHECK_GL_ERROR();
}

void IntGfxProgramComponent::SetInt(int32 a)
{
	mCount = 1;
	mValues[0] = a;

	if(mProgram.Applied)
		IntGfxProgramComponent::Apply();
}

void IntGfxProgramComponent::SetInt(int32 a, int32 b)
{
	mCount = 2;
	mValues[0] = a;
	mValues[1] = b;

	if(mProgram.Applied)
		IntGfxProgramComponent::Apply();
}

void IntGfxProgramComponent::SetInt(int32 a, int32 b, int32 c)
{
	mCount = 3;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;

	if(mProgram.Applied)
		IntGfxProgramComponent::Apply();
}

void IntGfxProgramComponent::SetInt(int32 a, int32 b, int32 c, int32 d)
{
	mCount = 4;
	mValues[0] = a;
	mValues[1] = b;
	mValues[2] = c;
	mValues[3] = d;

	if(mProgram.Applied)
		IntGfxProgramComponent::Apply();
}
