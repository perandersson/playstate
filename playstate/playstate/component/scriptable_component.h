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

	// Scriptable
	protected:
		virtual void OnRegistered();

	private:
		uint32 mUpdateFunc;
	};
	
	extern int Component_Init(lua_State* L);
	static luaL_Reg Component_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, Component_Init },
		{ NULL, NULL }
	};
}
