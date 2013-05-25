#pragma once

#include "../math/matrix4x4.h"
#include "../math/vector2.h"
#include "../math/color.h"
#include "../types.h"
#include "state/state_policy.h"
#include "texture2d.h"

namespace playstate
{
	//
	// Interface for components available in shaders - such as uniform variables in OpenGL.
	class IGfxProgramComponent
	{
	public:
		virtual ~IGfxProgramComponent() {}

		virtual void Apply() = 0;

		virtual void SetFloat(float32 a) = 0;
		virtual void SetFloat(float32 a, float32 b) = 0;
		virtual void SetFloat(float32 a, float32 b, float32 c) = 0;
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d) = 0;

		virtual void SetInt(int32 a) = 0;
		virtual void SetInt(int32 a, int32 b) = 0;
		virtual void SetInt(int32 a, int32 b, int32 c) = 0;
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d) = 0;
		
		virtual void SetTexture(Texture2D* texture) = 0;
		virtual void SetTexture(Resource<Texture2D>& texture) = 0;

		virtual void SetColorRGBA(const Color& color) = 0;
		virtual void SetColorRGB(const Color& color) = 0;
		virtual void SetMatrix(const Matrix4x4& matrix) = 0;
		virtual void SetVector3(const Vector3& vec) = 0;
		virtual void SetVector2(const Vector2& vec) = 0;
	};
}
