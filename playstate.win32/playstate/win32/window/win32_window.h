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
			void HandleEvents();

		// IWindowManager
		public:
			virtual const Point& GetSize() const;
			virtual void SetSize(const Point& size);

			virtual const playstate::string& GetTitle() const;
			virtual void SetTitle(const playstate::string& title);

			virtual void AddWindowResizedListener(IWindowResizedListener* listener);
			virtual void RemoveWindowResizedListener(IWindowResizedListener* listener);

			virtual void AddWindowClosedListener(IWindowClosedListener* listener);
			virtual void RemoveWindowClosedListener(IWindowClosedListener* listener);
		
			virtual float32 GetTimeSinceLastUpdate() const;

			
		private:
			HINSTANCE mAppInstance;
			HWND mWindowHandle;
			MSG mMessageQueue;

			WindowResizedListeners mWindowResizeListeners;
			WindowClosedListeners mWindowClosedListeners;

			playstate::string mWindowTitle;

			Point mSize;
			Point mPrevSize;

			float32 mTimeSinceLastUpdate;
			LONGLONG mLastTime;
			float64 mFrequency;

			DWORD mStyle;
			DWORD mExStyle;
		};
	}
}
