#pragma once

#include "../singleton.h"
#include "../types.h"
#include "../script/scriptable.h"

#include "window_closed_listener.h"
#include "window_resized_listener.h"

namespace playstate
{
	class IWindow : public Singleton<IWindow>
	{
	public:
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual void Resize(uint32 width, uint32 height) = 0;

		virtual const playstate::string& GetTitle() const = 0;
		virtual void SetTitle(const playstate::string& title) = 0;

		virtual void AddWindowClosedListener(IWindowClosedListener* listener) = 0;
		virtual void RemoveWindowClosedListener(IWindowClosedListener* listener) = 0;

		virtual void AddWindowResizedListener(IWindowResizedListener* listener) = 0;
		virtual void RemoveWindowResizedListener(IWindowResizedListener* listener) = 0;

		virtual float32 GetTimeSinceLastUpdate() const = 0;
	};

	
	//
	// Scripts
	//
	extern int IWindow_AddWindowClosedListener(lua_State* L);
	extern int IWindow_SetTitle(lua_State* L);
	extern int IWindow_GetWidth(lua_State* L);
	extern int IWindow_GetHeight(lua_State* L);
	extern int IWindow_Close(lua_State* L);
	static luaL_Reg IWindow_Methods[] = {
		{ "AddWindowClosedListener", IWindow_AddWindowClosedListener },
		{ "SetTitle", IWindow_SetTitle },
		{ "GetWidth", IWindow_GetWidth },
		{ "GetHeight", IWindow_GetHeight },
		{ "Close", IWindow_Close },
		{ NULL, NULL }
	};
	
	extern int IWindowClosedListener_Override(lua_State* L);
	static luaL_Reg IWindowClosedListener_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, IWindowClosedListener_Override },
		{ NULL, NULL }
	};
}
