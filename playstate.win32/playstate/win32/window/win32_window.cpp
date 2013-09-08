#include <playstate/memory/memory.h>
//#include "../../game/game_runner.h"
#include "win32_window.h"
#include <algorithm>

using namespace playstate;
using namespace playstate::win32;

// Anonymouse code
namespace {
	Win32Window* _window = NULL;
	LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		return _window->HandleEvent(hwnd, message, wparam, lparam);
	}
}

Win32Window::Win32Window(HINSTANCE hInstance) : mAppInstance(hInstance), 
	mWindowHandle(NULL), mSize(320, 240), mPrevSize(320, 240), mWindowTitle(SAFE_STRING("playstate"))
{
	_window = this;
	memset(&mMessageQueue, 0, sizeof(MSG));

	// Register the class used by the 
	WNDCLASSEX windowProperties = {0};
	windowProperties.cbSize = sizeof(WNDCLASSEX);
	windowProperties.style = CS_HREDRAW | CS_VREDRAW;
	windowProperties.lpfnWndProc = WindowsWindowWindowProc;
	windowProperties.hInstance = mAppInstance;
	windowProperties.lpszClassName = SAFE_STRING("playstate.Game");
	windowProperties.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassEx(&windowProperties);

	mExStyle = WS_EX_CLIENTEDGE;
	mStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;

	RECT windowSize = {0, 0, mSize.X, mSize.Y};
	AdjustWindowRectEx(&windowSize, mStyle, false, mExStyle);

	mWindowHandle = CreateWindowEx(mExStyle, SAFE_STRING("playstate.Game"), mWindowTitle.c_str(), mStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowSize.right - windowSize.left, windowSize.bottom - windowSize.top,
		NULL, NULL, mAppInstance, NULL);
	
	if(mWindowHandle != NULL) {	
		ShowWindow(mWindowHandle, SW_SHOW);
		UpdateWindow(mWindowHandle);
	}
}

Win32Window::~Win32Window()
{
	mWindowResizeListeners.clear();
	mWindowClosedListeners.clear();

	if(mWindowHandle != NULL)
	{
		CloseWindow(mWindowHandle);
		mWindowHandle = NULL;
	}

	UnregisterClass(SAFE_STRING("playstate.Game"), mAppInstance);
	mAppInstance = NULL;
}

const Point& Win32Window::GetSize() const 
{
	return mSize;
}

void Win32Window::SetSize(const Point& size)
{
	uint32 width = size.X > 0 ? size.X : 1;
	uint32 height = size.Y > 0 ? size.Y : 1;

	mSize.X = width;
	mSize.Y = height;

	RECT windowRect = {0, 0, mSize.X, mSize.Y};
	AdjustWindowRectEx(&windowRect, mStyle, FALSE, mExStyle);

	SetWindowPos(mWindowHandle, NULL, 0, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, SWP_NOMOVE);

	WindowResizedListeners::size_type numListeners = mWindowResizeListeners.size();
	for(WindowResizedListeners::size_type i = 0; i < numListeners; ++i) {
		mWindowResizeListeners[i]->OnWindowResized(mSize);
	}

	mPrevSize = mSize;
}

const playstate::string& Win32Window::GetTitle() const
{
	return mWindowTitle;
}

void Win32Window::SetTitle(const playstate::string& title)
{
	mWindowTitle = title;
	SetWindowText(mWindowHandle, title.c_str());
}

HWND Win32Window::GetWindowHandle() const
{
	return mWindowHandle;
}

LRESULT CALLBACK Win32Window::HandleEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch(message)
    {
    case WM_ACTIVATE:
		break;
    case WM_DESTROY:
        PostQuitMessage(WM_QUIT);
        break;
	case WM_EXITSIZEMOVE:
		{
			RECT rect;
			if(!GetClientRect(mWindowHandle, &rect))
				break;

			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;
		
			// Prevent the width and height to be 0 or smaller.
			width = width <= 0 ? 1 : width;
			height = height <= 0 ? 1 : height;

			mSize.X = width;
			mSize.Y = height;

			break;
		}
    default:
		return DefWindowProc(hwnd, message, wparam, lparam);
    }

	return 0;
}

void Win32Window::HandleEvents()
{	
	if(PeekMessage(&mMessageQueue, NULL, 0, 0, PM_REMOVE)) {
		if(mMessageQueue.message == WM_QUIT) {
			WindowClosedListeners::size_type size = mWindowClosedListeners.size();
			for(WindowClosedListeners::size_type i = 0; i < size; ++i) {
				if(mWindowClosedListeners[i]->OnWindowClosing() == false) {
					// TODO: IGNORE CLOSE!!!
				}
			}
			return;
		}
            
		TranslateMessage(&mMessageQueue);
		DispatchMessage(&mMessageQueue);
	}

	// Dispatch resize events if a resize has occured.
	if(mSize != mPrevSize) {
	//if(mWidth != mPrevWidth || mHeight != mPrevHeight) {
		mPrevSize = mSize;

		WindowResizedListeners::size_type size = mWindowResizeListeners.size();
		for(WindowResizedListeners::size_type i = 0; i < size; ++i) {
			mWindowResizeListeners[i]->OnWindowResized(mSize);
		}
	}
}


void Win32Window::AddWindowResizedListener(IWindowResizedListener* listener)
{
	mWindowResizeListeners.push_back(listener);
}

void Win32Window::RemoveWindowResizedListener(IWindowResizedListener* listener)
{
	WindowResizedListeners::iterator it = std::find(mWindowResizeListeners.begin(), mWindowResizeListeners.end(), listener);
	if(it != mWindowResizeListeners.end())
		mWindowResizeListeners.erase(it);
}

void Win32Window::AddWindowClosedListener(IWindowClosedListener* listener)
{
	mWindowClosedListeners.push_back(listener);
}

void Win32Window::RemoveWindowClosedListener(IWindowClosedListener* listener)
{
	WindowClosedListeners::iterator it = std::find(mWindowClosedListeners.begin(), mWindowClosedListeners.end(), listener);
	if(it != mWindowClosedListeners.end()) 
		mWindowClosedListeners.erase(it);
}
