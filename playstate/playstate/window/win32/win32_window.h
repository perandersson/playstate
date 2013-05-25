#pragma once

#include "../window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

namespace playstate
{
	class Win32Window : public IWindow
	{
		typedef std::vector<IWindowResizedListener*> WindowResizedListeners;
		typedef std::vector<IWindowClosedListener*> WindowClosedListeners;

	public:
		Win32Window(HINSTANCE hInstance, uint32 width, uint32 height, const std::string& title);
		virtual ~Win32Window();
	
		HWND GetWindowHandle() const;
		LRESULT CALLBACK HandleEvent(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
		
	// IWindowManager
	public:
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

	//
	// Scripts
	//
	extern int IWindow_AddWindowClosedListener(lua_State* L);
	extern int IWindow_SetTitle(lua_State* L);
	extern int IWindow_GetWidth(lua_State* L);
	extern int IWindow_GetHeight(lua_State* L);
	static luaL_Reg IWindow_Methods[] = {
		{ "AddWindowClosedListener", IWindow_AddWindowClosedListener },
		{ "SetTitle", IWindow_SetTitle },
		{ "GetWidth", IWindow_GetWidth },
		{ "GetHeight", IWindow_GetHeight },
		{ NULL, NULL }
	};
	
	extern int IWindowClosedListener_Override(lua_State* L);
	extern int IWindowClosedListener_GetID(lua_State* L);
	extern int IWindowClosedListener_OnWindowClosing(lua_State* L);
	static luaL_Reg IWindowClosedListener_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, IWindowClosedListener_Override },
		{ LUA_TOSTRING, IWindowClosedListener_GetID },
		{ "GetID", IWindowClosedListener_GetID },
		{ "OnWindowClosing", IWindowClosedListener_OnWindowClosing },
		{ NULL, NULL }
	};
}
