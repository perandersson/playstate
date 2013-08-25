#pragma once
#include "component.h"
#include "../processor/updatable.h"
#include "../processor/renderable.h"
#include "../processor/tickable.h"
#include "../script/scriptable.h"

namespace playstate
{
	class ScriptableComponent : public Component, public Updatable, public Renderable, public Tickable, public Scriptable
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

	// Renderable
	public:
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet);

	// Tickable
	public:
		virtual void Tick();

	// Scriptable
	public:
		virtual void OnRegistered();

	private:
		ScriptMethod* mOnComponentAdded;
		ScriptMethod* mOnComponentRemoved;
		ScriptMethod* mUpdate;
		ScriptMethod* mTick;
	};
	
	extern int Component_Init(lua_State* L);
	extern int Component_GetNode(lua_State* L);
	extern int Component_TranslateNode(lua_State* L);
	extern int Component_SetNodePosition(lua_State* L);
	extern int Component_SetNodeRotation(lua_State* L);
	extern int Component_GetNodePosition(lua_State* L);
	extern int Component_Show(lua_State* L);
	extern int Component_Hide(lua_State* L);
	static luaL_Reg Component_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, Component_Init },
		{ "GetNode", Component_GetNode },
		{ "TranslateNode", Component_TranslateNode },
		{ "SetNodePosition", Component_SetNodePosition },
		{ "SetNodeRotation", Component_SetNodeRotation },
		{ "GetNodePosition", Component_GetNodePosition },
		{ "Show", Component_Show },
		{ "Hide", Component_Hide },
		{ NULL, NULL }
	};
}
