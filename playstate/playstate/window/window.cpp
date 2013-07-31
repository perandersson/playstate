#include "../memory/memory.h"
#include "window.h"
using namespace playstate;

template<> playstate::IWindow* playstate::Singleton<playstate::IWindow>::gSingleton = NULL;

class ScriptWindowClosedListener : public IWindowClosedListener, public Scriptable
{
public:
	ScriptWindowClosedListener() {}
	virtual ~ScriptWindowClosedListener() {}
		
	virtual void OnRegistered() {
		mOnWindowClosingID = GetMethodID("OnWindowClosing");
	}

	virtual bool OnWindowClosing() {
		if(PrepareMethod(mOnWindowClosingID)) {
			if(lua_pcall(mCurrentState, 1, 1, NULL) == 0) {
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

namespace playstate
{
	int IWindow_AddWindowClosedListener(lua_State* L)
	{
		ScriptWindowClosedListener* listener = luaM_popobject<ScriptWindowClosedListener>(L);
		if(listener != NULL) {
			IWindow::Get().AddWindowClosedListener(listener);
		}
		return 0;
	}
	
	int IWindow_SetTitle(lua_State* L)
	{
		int top = lua_gettop(L);
		std::string title = lua_tostring(L, -1); lua_pop(L, 1);
		IWindow::Get().SetTitle(title);
		return 0;
	}
	
	int IWindow_GetWidth(lua_State* L)
	{
		lua_pushnumber(L, IWindow::Get().GetWidth());
		return 1;
	}

	int IWindow_GetHeight(lua_State* L)
	{
		lua_pushnumber(L, IWindow::Get().GetHeight());
		return 1;
	}

	int IWindow_Close(lua_State* L)
	{
		//_window->Close();
		return 0;
	}

	/////////////////////////////////////////////////////

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
}