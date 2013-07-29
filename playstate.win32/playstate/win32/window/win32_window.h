#pragma once

#include <playstate/window/window.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

namespace playstate
{
	namespace win32
	{
		class Win32Window : public IWindow
		{
			typedef std::vector<IWindowResizedListener*> WindowResizedListeners;
			typedef std::vector<IWindowClosedListener*> WindowClosedListeners;

		public:
			Win32Window(HINSTANCE hInstance);
			virtual ~Win32Window();
	
			HWND GetWindowHandle() const;
			LRESULT CALLBACK HandleEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		
		// IWindowManager
		public:
			virtual void Open(uint32 width, uint32 height, const std::string& title);
			virtual uint32 GetWidth() const;
			virtual uint32 GetHeight() const;
			virtual void Resize(uint32 width, uint32 height);

			virtual const std::string& GetTitle() const;
			virtual void SetTitle(const std::string& title);

			virtual void AddWindowResizedListener(IWindowResizedListener* listener);
			virtual void RemoveWindowResizedListener(IWindowResizedListener* listener);

			virtual void AddWindowClosedListener(IWindowClosedListener* listener);
			virtual void RemoveWindowClosedListener(IWindowClosedListener* listener);
		
			virtual float32 GetTimeSinceLastUpdate() const;

			virtual void HandleEvents();

		private:
			HINSTANCE mAppInstance;
			HWND mWindowHandle;
			MSG mMessageQueue;

			WindowResizedListeners mWindowResizeListeners;
			WindowClosedListeners mWindowClosedListeners;

			std::string mWindowTitle;

			uint32 mWidth;
			uint32 mHeight;

			uint32 mPrevWidth;
			uint32 mPrevHeight;

			float32 mTimeSinceLastUpdate;
			LONGLONG mLastTime;
			float64 mFrequency;
		};
	}
}
