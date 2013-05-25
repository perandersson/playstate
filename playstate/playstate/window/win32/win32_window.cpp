#include "../../memory/memory.h"
#include "../../game/game_runner.h"
#include "win32_window.h"
#include <algorithm>
using namespace playstate;

template<> playstate::IWindow* playstate::Singleton<playstate::IWindow>::gSingleton = NULL;

// Anonymouse code
namespace {
	Win32Window* _window = NULL;
	LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
	{
		return _window->HandleEvent(hwnd, message, wparam, lparam);
	}
}

Win32Window::Win32Window(HINSTANCE hInstance, uint32 width, uint32 height, const std::string& title) : mAppInstance(hInstance), 
	mWindowHandle(NULL), mWidth(width), mPrevWidth(width), mHeight(height), mPrevHeight(height), mWindowTitle(title),
	mTimeSinceLastUpdate(0.0f), mLastTime(0)
{
	_window = this;
	memset(&mMessageQueue, 0, sizeof(MSG));

	// Register the class used by the 
	WNDCLASSEX windowProperties = {0};
	windowProperties.cbSize = sizeof(WNDCLASSEX);
	windowProperties.style = CS_HREDRAW | CS_VREDRAW;
	windowProperties.lpfnWndProc = WindowsWindowWindowProc;
	windowProperties.hInstance = mAppInstance;
	windowProperties.lpszClassName = "playstate.Game";
	windowProperties.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_SHARED);
	RegisterClassEx(&windowProperties);

	DWORD winStyleEx = WS_EX_CLIENTEDGE;
	DWORD winStyle = WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW;

	RECT windowSize = {0, 0, width, height};
	AdjustWindowRectEx(&windowSize, winStyle, false, winStyleEx);

	mWindowHandle = CreateWindowEx(winStyleEx, "playstate.Game", title.c_str(), winStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		windowSize.right - windowSize.left, windowSize.bottom - windowSize.top,
		NULL, NULL, mAppInstance, NULL);
	
	if(mWindowHandle != NULL) {
		LARGE_INTEGER performanceFrequency;
		QueryPerformanceFrequency(&performanceFrequency);
		mFrequency = (double)(performanceFrequency.QuadPart);
	
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

	UnregisterClass("playstate.Game", mAppInstance);
	mAppInstance = NULL;
}

uint32 Win32Window::GetWidth() const 
{
	return mWidth;
}

uint32 Win32Window::GetHeight() const 
{
	return mHeight;
}

void Win32Window::Resize(uint32 width, uint32 height)
{
	mWidth = width > 0 ? width : 1;
	mHeight = height > 0 ? height : 1;

	RECT rcClient;
	SetWindowPos(mWindowHandle, NULL, 0, 0, width, height, SWP_NOMOVE);
	GetClientRect(mWindowHandle, &rcClient);

	WindowResizedListeners::size_type size = mWindowResizeListeners.size();
	for(WindowResizedListeners::size_type i = 0; i < size; ++i) {
		mWindowResizeListeners[i]->OnWindowResized(*this, mWidth, mHeight);
	}

	mPrevWidth = rcClient.right - rcClient.left;
	mPrevHeight = rcClient.bottom - rcClient.top;
}

const std::string& Win32Window::GetTitle() const
{
	return mWindowTitle;
}

void Win32Window::SetTitle(const std::string& title)
{
	mWindowTitle = title;
	SetWindowText(mWindowHandle, title.c_str());
}

float32 Win32Window::GetTimeSinceLastUpdate() const
{
	return mTimeSinceLastUpdate;
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
			if(!GetClientRect(hwnd, &rect))
				break;

			int width = rect.right - rect.left;
			int height = rect.bottom - rect.top;

			if(width == mPrevWidth && height == mPrevHeight)
				break;
			
			// Prevent the width and height to be 0 or smaller.
			width = width <= 0 ? 1 : width;
			height = height <= 0 ? 1 : height;

			mWidth = width;
			mHeight = height;

			WindowResizedListeners::size_type size = mWindowResizeListeners.size();
			for(WindowResizedListeners::size_type i = 0; i < size; ++i) {
				mWindowResizeListeners[i]->OnWindowResized(*this, width, height);
			}
		}
		
		break;
    default:
		return DefWindowProc(hwnd, message, wparam, lparam);
    }

	return 0;
}

void Win32Window::HandleEvents()
{	
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	if(mLastTime > 0)
	{
		LONGLONG quadPartDelta = now.QuadPart - mLastTime;
		double dt = double(quadPartDelta) / mFrequency;
		mTimeSinceLastUpdate = (float)(dt);
	}
	mLastTime = now.QuadPart;
	/*
	mTotalTimeThisSecond += mTimeSinceLastUpdate;
	mCurrentFrameNumber++;
	if(mTotalTimeThisSecond > 1.0f) {
		mTotalTimeThisSecond -= 1.0f;
		mFps = mCurrentFrameNumber;
		mCurrentFrameNumber = 0;

		char title[64];
		sprintf(title, "Snowpeak: %d", mFps);
		SetWindowTextA(mWindowHandle, title);
	}
	*/

	if(PeekMessage(&mMessageQueue, NULL, 0, 0, PM_REMOVE))
	{
		if(mMessageQueue.message == WM_QUIT)
		{
			WindowClosedListeners::size_type size = mWindowClosedListeners.size();
			for(WindowClosedListeners::size_type i = 0; i < size; ++i) {
				if(mWindowClosedListeners[i]->OnWindowClosing(*this) == false) {
					// TODO: IGNORE CLOSE!!!
				}
			}
			return;
		}
            
		TranslateMessage(&mMessageQueue);
		DispatchMessage(&mMessageQueue);
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

namespace playstate
{
	class ScriptWindowClosedListener : public IWindowClosedListener, public Scriptable
	{
	public:
		ScriptWindowClosedListener() {}
		virtual ~ScriptWindowClosedListener() {}
		
		virtual void OnRegistered() {
			mOnWindowClosingID = GetMethodID("OnWindowClosing");
		}

		virtual bool OnWindowClosing(IWindow& window) {
			if(PrepareMethod(mOnWindowClosingID)) {
				lua_rawgeti(mCurrentState, LUA_REGISTRYINDEX, window.GetID());
				if(lua_pcall(mCurrentState, 2, 1, NULL) == 0) {
					bool ret = lua_toboolean(mCurrentState, -1) == 1; lua_pop(mCurrentState, -1);
					return ret;
				} else {
					const char* err = lua_tostring(mCurrentState, -1);
					lua_pop(mCurrentState, 1);
				}
			}
			return true;
		}

	private:
		uint32 mOnWindowClosingID;
	};
	
	/////////////////////////////////////////////////

	int IWindow_AddWindowClosedListener(lua_State* L)
	{
		ScriptWindowClosedListener* listener = luaM_popobject<ScriptWindowClosedListener>(L);
		Win32Window* window = static_cast<Win32Window*>(&IWindow::Get());
		if(listener != NULL) {
			window->AddWindowClosedListener(listener);
		}
		return 0;
	}
	
	int IWindow_SetTitle(lua_State* L)
	{
		int top = lua_gettop(L);
		std::string title = lua_tostring(L, -1); lua_pop(L, 1);
		Win32Window* window = static_cast<Win32Window*>(&IWindow::Get());
		window->SetTitle(title);
		return 0;
	}
	
	int IWindow_GetWidth(lua_State* L)
	{
		Win32Window* window = static_cast<Win32Window*>(&IWindow::Get());
		lua_pushnumber(L, window->GetWidth());
		return 1;
	}

	int IWindow_GetHeight(lua_State* L)
	{
		Win32Window* window = static_cast<Win32Window*>(&IWindow::Get());
		lua_pushnumber(L, window->GetHeight());
		return 1;
	}

	/////////////////////////////////////////////////////

	int IWindowClosedListener_GetID(lua_State* L)
	{
		ScriptWindowClosedListener* listener = luaM_popobject<ScriptWindowClosedListener>(L);
		if(listener == NULL)
			return LUA_NOREF;

		return listener->GetID();
	}

	int IWindowClosedListener_Override(lua_State* L)
	{
		if(lua_istable(L, -1) == 0) {
			lua_pop(L, 1);
			return 0;
		}
		
		ScriptWindowClosedListener* listener = new ScriptWindowClosedListener();
		luaM_setinstance(L, listener);
		
		const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		listener->RegisterObject(L, ref);
		int top1 = lua_gettop(L);

		return 0;
	}
	
	int IWindowClosedListener_OnWindowClosing(lua_State* L)
	{
		ScriptWindowClosedListener* listener = luaM_popobject<ScriptWindowClosedListener>(L);
		Win32Window* window = luaM_popobject<Win32Window>(L);
		if(listener != NULL && window != NULL) {
			bool res = listener->OnWindowClosing(*window);
			lua_pushboolean(L, res ? 1 : 0);
			return 1;
		}
		lua_pushboolean(L, 0);
		return 1;
	}
}