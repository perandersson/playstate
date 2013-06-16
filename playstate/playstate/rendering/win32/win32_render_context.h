#pragma once
#include "../render_context.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	class Win32RenderContext : public IRenderContext
	{
	public:
		Win32RenderContext(HDC deviceContext, HGLRC context);
		virtual ~Win32RenderContext();
		
	// IRenderContext
	public:
		virtual void MakeCurrent();
		virtual void Unbind();
		virtual void SwapBuffers();

	public:
		// Read-only property for the render context
		HGLRC& const RenderContext;

	private:
		HDC mDeviceContext;
		HGLRC mRenderContext;
	};
}
