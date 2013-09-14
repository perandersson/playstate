#pragma once

#include "default_gfx_program_component.h"
#include <gl/glew.h>

namespace playstate
{
	class OGL3GfxProgram;

	class FloatGfxProgramComponent : public DefaultGfxProgramComponent
	{
	public:
		FloatGfxProgramComponent(OGL3GfxProgram& program, GLint componentId, GLenum type);
		virtual ~FloatGfxProgramComponent();

	// IGfxProgramComponent
	public:
		virtual void Apply();
		virtual void SetFloat(float32 a);
		virtual void SetFloat(float32 a, float32 b);
		virtual void SetFloat(float32 a, float32 b, float32 c);
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d);
		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGB(const Color& color);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);
		virtual void MarkAsDirty();

	private:
		OGL3GfxProgram& mProgram;
		GLint mComponentId;
		GLenum mType;
		
		int32 mCount;
		float32 mValues[4];
		float32 mValuesSet[4];
	};

}
