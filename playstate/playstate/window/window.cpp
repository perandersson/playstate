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
		mOnWindowClosing = std::auto_ptr<ScriptMethod>(GetMethod("OnWindowClosing"));
	}

	virtual bool OnWindowClosing() {
		if(mOnWindowClosing->Invoke()) {
			return mOnWindowClosing->GetBool();
		}
		return true;
	}

private:
	std::auto_ptr<ScriptMethod> mOnWindowClosing;
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
		playstate::string title = lua_tostring(L, -1); lua_pop(L, 1);
		IWindow::Get().SetTitle(title);
		return 0;
	}
	
	int IWindow_GetSize(lua_State* L)
	{
		return luaM_pushpoint(L, IWindow::Get().GetSize());
	}
	
	int IWindow_SetSize(lua_State* L)
	{
		Point size = luaM_poppoint(L);
		IWindow::Get().SetSize(size);
		return 0;
	}
	
	int IWindow_GetWidth(lua_State* L)
	{
		lua_pushinteger(L, IWindow::Get().GetSize().X);
		return 1;
	}

	int IWindow_GetHeight(lua_State* L)
	{
		lua_pushinteger(L, IWindow::Get().GetSize().Y);
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