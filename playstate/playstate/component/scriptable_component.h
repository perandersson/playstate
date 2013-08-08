#pragma once
#include "component.h"
#include "../processor/updatable.h"
#include "../script/scriptable.h"

namespace playstate
{
	class ScriptableComponent : public Component, public Updatable, public Scriptable
	{
	public:
		ScriptableComponent(uint32 type);
		~ScriptableComponent();

	// Component
	protected:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

	// Updatable
	public:
		virtual void Update();
	};
	
	extern int Component_Init(lua_State* L);
	extern int Component_GetNode(lua_State* L);
	extern int Component_TranslateNode(lua_State* L);
	extern int Component_SetNodePosition(lua_State* L);
	extern int Component_SetNodeRotation(lua_State* L);
	extern int Component_GetNodePosition(lua_State* L);
	static luaL_Reg Component_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, Component_Init },
		{ "GetNode", Component_GetNode },
		{ "TranslateNode", Component_TranslateNode },
		{ "SetNodePosition", Component_SetNodePosition },
		{ "SetNodeRotation", Component_SetNodeRotation },
		{ "GetNodePosition", Component_GetNodePosition },
		{ NULL, NULL }
	};
}
