#pragma once

#include "../gfx_program_component.h"

namespace playstate
{
	class DefaultGfxProgramComponent : public IGfxProgramComponent
	{
	protected:
		DefaultGfxProgramComponent();
		virtual ~DefaultGfxProgramComponent();

	public:
		virtual void SetFloat(float32 a);
		virtual void SetFloat(float32 a, float32 b);
		virtual void SetFloat(float32 a, float32 b, float32 c);
		virtual void SetFloat(float32 a, float32 b, float32 c, float32 d);

		virtual void SetInt(int32 a);
		virtual void SetInt(int32 a, int32 b);
		virtual void SetInt(int32 a, int32 b, int32 c);
		virtual void SetInt(int32 a, int32 b, int32 c, int32 d);
		
		virtual void SetTexture(Texture2D* texture);
		virtual void SetTexture(Resource<Texture2D>& texture);

		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGB(const Color& color);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);
	};

}
