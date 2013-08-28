#pragma once
#include <playstate/rendering/render_context.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32RenderContext : public IRenderContext
		{
		public:
			Win32RenderContext(HDC deviceContext, HGLRC context);
			virtual ~Win32RenderContext();

			inline HGLRC GetRenderContext() const {
				return mRenderContext;
			}
		
		// IRenderContext
		public:
			virtual void MakeCurrent();
			virtual void Unbind();
			virtual void SwapBuffers();

		private:
			HDC mDeviceContext;
			HGLRC mRenderContext;
		};
	}
}
