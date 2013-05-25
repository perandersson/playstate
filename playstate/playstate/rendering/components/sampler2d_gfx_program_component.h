#pragma once

#include "default_gfx_program_component.h"
#include "../state/state_policy.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	class Sampler2DGfxProgramComponent : public DefaultGfxProgramComponent
	{
		enum {
			TEXTURE_BIT = BIT(1),
			ACTIVE_TEXTURE_BIT = BIT(2),
			UNIFORM_BIT = BIT(3),
		};

	public:
		Sampler2DGfxProgramComponent(GfxProgram& program, GLint componentId, MinFilter::Enum minFilter, MagFilter::Enum magFilter,
			TextureWrap::Enum ws, TextureWrap::Enum wt);
		virtual ~Sampler2DGfxProgramComponent();

	// IGfxProgramComponent
	public:
		virtual void Apply();
		virtual void SetTexture(Texture2D* texture);

	private:
		GfxProgram& mProgram;
		GLint mComponentId;
		uint32 mDirty;

		Texture2D* mTexture;
		GLuint mActiveTexture;

		MinFilter::Enum mMinFilter;
		MagFilter::Enum mMagFilter;
		TextureWrap::Enum mWS;
		TextureWrap::Enum mWT;
	};
}
