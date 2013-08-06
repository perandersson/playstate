#include "../../memory/memory.h"
#include "sampler2d_gfx_program_component.h"
#include "../texture2d.h"
#include "../gfx_program.h"

#include <cassert>

using namespace playstate;

namespace {
	int32 _current_textureIndex = 0; 
}

Sampler2DGfxProgramComponent::Sampler2DGfxProgramComponent(GfxProgram& program, GLint componentId, MinFilter::Enum minFilter, MagFilter::Enum magFilter,
	TextureWrap::Enum ws, TextureWrap::Enum wt) : mProgram(program), mComponentId(componentId), mDirty(UNIFORM_BIT), mActiveTexture(0), mTexture(NULL),
	mMinFilter(minFilter), mMagFilter(magFilter), mWS(ws), mWT(wt)
{
	mActiveTexture = _current_textureIndex++;
	if(_current_textureIndex > MaxActiveTextures)
		_current_textureIndex = 0;
}

Sampler2DGfxProgramComponent::~Sampler2DGfxProgramComponent()
{
}

void Sampler2DGfxProgramComponent::Apply()
{
	if(mTexture == NULL) {
		return;
	}

	if(BIT_ISSET(mDirty, TEXTURE_BIT)) {
		mTexture->Bind(mActiveTexture, mMinFilter, mMagFilter, mWS, mWT);
	}

	if(BIT_ISSET(mDirty, UNIFORM_BIT)) {
		glUniform1i(mComponentId, mActiveTexture);
	}

	mDirty = 0;
	CHECK_GL_ERROR();
}

void Sampler2DGfxProgramComponent::SetTexture(Texture2D* texture)
{
	const uint32 stateId = mTexture != NULL ? mTexture->GetUUID() : 0;
	mTexture = texture;
	if(texture == NULL) {
		return;
	}
	
	if(texture->GetUUID() != stateId)
		BIT_SET(mDirty, TEXTURE_BIT);

	if(mProgram.IsApplied())
		Sampler2DGfxProgramComponent::Apply();
}

void Sampler2DGfxProgramComponent::MarkAsDirty()
{
	mDirty = TEXTURE_BIT | ACTIVE_TEXTURE_BIT | UNIFORM_BIT;

	// TODO: Make sure that the textures are marked as dirty correctly.
}
