#include "../../memory/memory.h"
#include "sampler2d_gfx_program_component.h"
#include "../texture2d.h"
#include "../gfx_program.h"

#include <cassert>

using namespace playstate;

Sampler2DGfxProgramComponent::Sampler2DGfxProgramComponent(GfxProgram& program, GLint componentId, uint32 activeTexture, MinFilter::Enum minFilter, MagFilter::Enum magFilter,
	TextureWrap::Enum ws, TextureWrap::Enum wt) : mProgram(program), mComponentId(componentId), mDirty(UNIFORM_BIT), mActiveTexture(activeTexture), mTexture(NULL),
	mMinFilter(minFilter), mMagFilter(magFilter), mWS(ws), mWT(wt)
{
}

Sampler2DGfxProgramComponent::~Sampler2DGfxProgramComponent()
{
}

void Sampler2DGfxProgramComponent::Apply()
{
	if(mTexture == NULL) {
		return;
	}
	
	mTexture->Bind(mActiveTexture);
	mTexture->UpdateFilters(mMinFilter, mMagFilter, mWS, mWT);
	
	if(BIT_ISSET(mDirty, UNIFORM_BIT)) {
		glUniform1i(mComponentId, mActiveTexture);
	}

	mDirty = 0;
}

void Sampler2DGfxProgramComponent::SetTexture(Texture2D* texture)
{
	if(texture == NULL) {
		mTexture = texture;
		return;
	}

	const uint32 prevTextureUUID = mTexture != NULL ? mTexture->GetUUID() : 0;
	if(texture->GetUUID() != prevTextureUUID)
		BIT_SET(mDirty, TEXTURE_BIT);
	mTexture = texture;

	if(mProgram.IsApplied())
		Sampler2DGfxProgramComponent::Apply();
}

void Sampler2DGfxProgramComponent::MarkAsDirty()
{
	mDirty = TEXTURE_BIT | ACTIVE_TEXTURE_BIT | UNIFORM_BIT;

	// TODO: Make sure that the textures are marked as dirty correctly.
}
