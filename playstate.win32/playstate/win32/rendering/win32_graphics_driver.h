#pragma once
#include <playstate/rendering/graphics_driver.h>
#include "../window/win32_window.h"

namespace playstate
{
	namespace win32
	{
		class Win32GraphicsDriver : public IGraphicsDriver
		{
		public:
			Win32GraphicsDriver(Win32Window& window);
			virtual ~Win32GraphicsDriver();

		// IGraphicsDriver
		public:
			virtual IRenderContext* CreateRenderContext();
			virtual IRenderContext* CreateRenderContext(IRenderContext* context);
			virtual IRenderContext* GetScreenRenderContext();

		private:
			HWND mWindowHandle;
			IRenderContext* mScreenRenderContext;
			HDC mDeviceContext;
		};
	}
}
