#pragma once
#include "../graphics_driver.h"
#include "../../window/win32/win32_window.h"

namespace playstate
{
	class Win32GraphicsDriver : public IGraphicsDriver
	{
	public:
		Win32GraphicsDriver(Win32Window& window);
		~Win32GraphicsDriver();

	// IGraphicsDriver
	public:
		virtual IRenderContext* CreateRenderContext();
		virtual IRenderContext* CreateRenderContext(IRenderContext* context);

	private:
		HWND mWindowHandle;
		IRenderContext* mScreenRenderContext;
		HDC mDeviceContext;
	};
}
