#pragma once

#include "../gfx_program_component.h"

namespace playstate
{
	class GfxProgramComponentNotFound : public IGfxProgramComponent
	{
	public:
		GfxProgramComponentNotFound();
		virtual ~GfxProgramComponentNotFound();

		virtual void Apply();

		virtual void SetFloat(float a);
		virtual void SetFloat(float a, float b);
		virtual void SetFloat(float a, float b, float c);
		virtual void SetFloat(float a, float b, float c, float d);

		virtual void SetInt(int a);
		virtual void SetInt(int a, int b);
		virtual void SetInt(int a, int b, int c);
		virtual void SetInt(int a, int b, int c, int d);
		
		virtual void SetTexture(ITexture2D* texture);
		virtual void SetTexture(Resource<ITexture2D>& texture);

		virtual void SetMatrix(const Matrix4x4& matrix);
		virtual void SetColorRGBA(const Color& color);
		virtual void SetColorRGB(const Color& color);
		virtual void SetVector3(const Vector3& vec);
		virtual void SetVector2(const Vector2& vec);
		virtual void MarkAsDirty();
	};
}
