#include "../memory/memory.h"
#include "scriptable_component.h"
#include "../scene/scene_node.h"
#include "../scene/scene_group.h"
using namespace playstate;

ScriptableComponent::ScriptableComponent(uint32 type)
	: Component(type), mUpdateFunc(0)
{
}

ScriptableComponent::~ScriptableComponent()
{
}

void ScriptableComponent::OnComponentAdded()
{
	if(mUpdateFunc != 0) {
		Updatable::Attach(GetNode()->GetGroup());
	}
	
	if(PrepareMethod("OnComponentAdded")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableComponent::OnComponentRemoved()
{
	if(mUpdateFunc != 0) {
		Updatable::Detach();
	}

	if(PrepareMethod("OnComponentRemoved")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableComponent::OnRegistered()
{
	mUpdateFunc = GetMethodID("Update");
}

void ScriptableComponent::Update()
{
	if(mUpdateFunc != 0 && PrepareMethod(mUpdateFunc)) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

namespace playstate
{
	int Component_Init(lua_State* L)
	{
		int params = lua_gettop(L);
		type_mask typeMask = BIT_ALL;
		if(params == 2) {
			typeMask = (type_mask)lua_tonumber(L, -1); lua_pop(L, 1);
		}

		ScriptableComponent* node = new ScriptableComponent(typeMask);
		luaM_setinstance(L, node);
		
		const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		node->RegisterObject(L, ref);
		return 1;
	}
	
	int Component_GetNode(lua_State* L)
	{
		ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
		if(component != NULL) {
			luaM_pushobject(L, "SceneNode", component->GetNode());
		} else {
			lua_pushnil(L);
		}

		return 1;
	}
	
	int Component_TranslateNode(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
		if(component != NULL) {
			SceneNode* owner = component->GetNode();
			owner->SetPosition(owner->GetPosition() + vec);
		}

		return 0;
	}

	int Component_SetNodePosition(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
		if(component != NULL) {
			SceneNode* owner = component->GetNode();
			owner->SetPosition(vec);
		}

		return 0;
	}

	int Component_SetNodeRotation(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
		if(component != NULL) {
			SceneNode* owner = component->GetNode();
			owner->SetRotation(vec);
		}

		return 0;
	}
	
	int Component_GetNodePosition(lua_State* L)
	{
		ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
		if(component != NULL) {
			luaM_pushvector3(L, component->GetNode()->GetPosition());
		} else {
			luaM_pushvector3(L, Vector3::Zero);
		}
		
		return 3;
	}
}

