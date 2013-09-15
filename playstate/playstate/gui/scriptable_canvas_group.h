#pragma once

#include "canvas_group.h"
#include "../processor/updatable.h"

namespace playstate
{
	class ScriptableCanvasGroup : public CanvasGroup, public Updatable, public Tickable
	{
	public:
		ScriptableCanvasGroup();
		virtual ~ScriptableCanvasGroup();

	// CanvasGroup
	public:
		virtual void OnProcessCanvas();
		virtual void OnAddedToCanvas();
		virtual void OnRemovingFromCanvas();

	// Updatable
	public:
		virtual void Update();
		
	// Tickable
	public:
		virtual void Tick();
		
	// Scriptable
	public:
		virtual void OnRegistered();

	private:
		ScriptMethod* mOnProcessCanvas;
		ScriptMethod* mUpdate;
		ScriptMethod* mTick;
	};

	//
	// Script integration
	//

	extern int CanvasGroup_Init(lua_State* L);
	extern int CanvasGroup_SetStyle(lua_State* L);
	extern int CanvasGroup_BeginFrame(lua_State* L);
	extern int CanvasGroup_EndFrame(lua_State* L);
	extern int CanvasGroup_Button(lua_State* L);
	extern int CanvasGroup_Toggle(lua_State* L);
	static luaL_Reg CanvasGroup_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, CanvasGroup_Init },
		{ "SetStyle", CanvasGroup_SetStyle },
		{ "BeginFrame", CanvasGroup_BeginFrame },
		{ "EndFrame", CanvasGroup_EndFrame },
		{ "Button", CanvasGroup_Button },
		{ "Toggle", CanvasGroup_Toggle },
		{ NULL, NULL }
	};
}
