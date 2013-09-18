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
		virtual void OnProcessCanvas(GuiGeometryBuilder* builder);
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
	extern int CanvasGroup_SetName(lua_State* L);
	extern int CanvasGroup_GetName(lua_State* L);
	extern int CanvasGroup_SetStyle(lua_State* L);
	extern int CanvasGroup_BeginFrame(lua_State* L);
	extern int CanvasGroup_EndFrame(lua_State* L);
	extern int CanvasGroup_Button(lua_State* L);
	extern int CanvasGroup_Checkbox(lua_State* L);
	extern int CanvasGroup_Slider(lua_State* L);
	extern int CanvasGroup_ComboBox(lua_State* L);
	static luaL_Reg CanvasGroup_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, CanvasGroup_Init },
		{ "SetName", CanvasGroup_SetName },
		{ "GetName", CanvasGroup_GetName },
		{ "SetStyle", CanvasGroup_SetStyle },
		// Gui control types
		{ "BeginFrame", CanvasGroup_BeginFrame },
		{ "EndFrame", CanvasGroup_EndFrame },
		{ "Button", CanvasGroup_Button },
		{ "Checkbox", CanvasGroup_Checkbox },
		{ "Slider", CanvasGroup_Slider },
		{ "ComboBox", CanvasGroup_ComboBox },
		{ NULL, NULL }
	};
}
