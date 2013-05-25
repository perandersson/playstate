#pragma once
#include "../render_system.h"
#include "../exception/rendering_exception.h"
#include "../../window/win32/win32_window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	//
	// Implementation that's responsible for swapping the frame buffer to the screen.
	class Win32RenderSystem : public RenderSystem
	{
	public:
		Win32RenderSystem(Win32Window& window, ScriptSystem& scriptSystem);
		~Win32RenderSystem();

	// RenderSystem
	public:
		virtual void Initialize();
		virtual void SwapBuffers();
		virtual IRenderContext* CreateRenderContext();

	private:
		HWND mWindowHandle;
		HGLRC mRenderContext;
		HDC mDeviceContext;
	};
}
