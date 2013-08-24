#include "../memory/memory.h"
#include "scriptable_component.h"
#include "../scene/scene_node.h"
#include "../scene/scene_group.h"
using namespace playstate;

ScriptableComponent::ScriptableComponent(uint32 type)
	: Component(type), Updatable(), Renderable(), Tickable(), Scriptable()
{
}

ScriptableComponent::~ScriptableComponent()
{
}

void ScriptableComponent::OnComponentAdded()
{
	if(HasMethod("Update")) {
		Updatable::Attach(GetNode()->GetGroup());
	}

	if(HasMethod("Collect")) {
		Renderable::Attach(GetNode()->GetGroup());
	}

	if(HasMethod("Tick")) {
		Tickable::Attach(GetNode()->GetGroup());
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
	Updatable::Detach();
	Renderable::Detach();
	Tickable::Detach();

	if(PrepareMethod("OnComponentRemoved")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableComponent::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
	/*if(PrepareMethod("Collect")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}*/
}

void ScriptableComponent::Update()
{
	if(PrepareMethod("Update")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableComponent::Tick()
{
	if(PrepareMethod("Tick")) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

int playstate::Component_Init(lua_State* L)
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
	return 0;
}
	
int playstate::Component_GetNode(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: self<Component>:GetNode()");
		lua_pushnil(L);
		return 1;
	}
		
	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		luaM_pushobject(L, "SceneNode", component->GetNode());
	} else {
		lua_pushnil(L);
	}

	return 1;
}
	
int playstate::Component_TranslateNode(lua_State* L)
{
	Vector3 vec = luaM_popvector3(L);
	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		SceneNode* owner = component->GetNode();
		owner->SetPosition(owner->GetPosition() + vec);
	}

	return 0;
}

int playstate::Component_SetNodePosition(lua_State* L)
{
	Vector3 vec = luaM_popvector3(L);
	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		SceneNode* owner = component->GetNode();
		owner->SetPosition(vec);
	}

	return 0;
}

int playstate::Component_SetNodeRotation(lua_State* L)
{
	if(lua_gettop(L) < 2) {
		luaM_printerror(L, "Expected: self<Component>:SetNodeRotation(Vector3)");
		luaM_pushvector3(L, Vector3::Zero);
		return 1;
	}

	Vector3 vec = luaM_popvector3(L);
	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		SceneNode* owner = component->GetNode();
		owner->SetRotation(vec);
	}

	return 0;
}
	
int playstate::Component_GetNodePosition(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: self<Component>:GetNodePosition()");
		luaM_pushvector3(L, Vector3::Zero);
		return 1;
	}
		
	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		luaM_pushvector3(L, component->GetNode()->GetPosition());
	} else {
		luaM_pushvector3(L, Vector3::Zero);
	}
		
	return 3;
}

int playstate::Component_Show(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: self<Component>:Show()");
		return 0;
	}

	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		component->Show();
	}

	return 0;
}

int playstate::Component_Hide(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: self<Component>:Hide()");
		return 0;
	}

	ScriptableComponent* component = luaM_popobject<ScriptableComponent>(L);
	if(component != NULL) {
		component->Hide();
	}

	return 0;
}
