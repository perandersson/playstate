#include "../memory/memory.h"
#include "scriptable_component.h"
#include "../scene/scene_node.h"
#include "../scene/scene_group.h"
using namespace playstate;

ScriptableComponent::ScriptableComponent(uint32 type)
	: Component(type), Updatable(), Renderable(), Tickable(), Scriptable(),
	mOnComponentAdded(NULL), mOnComponentRemoved(NULL), mOnEvent(NULL), mUpdate(NULL), mTick(NULL)
{
}

ScriptableComponent::~ScriptableComponent()
{
	delete mOnComponentAdded;
	delete mOnComponentRemoved;
	delete mOnEvent;
	delete mUpdate;
	delete mTick;
}

void ScriptableComponent::OnComponentAdded()
{
	if(mUpdate != NULL)
		Updatable::Attach(GetGroup());

	if(mTick != NULL)
		Tickable::Attach(GetGroup());

	if(mOnComponentAdded != NULL)
		mOnComponentAdded->Invoke();
}

void ScriptableComponent::OnComponentRemoved()
{
	Updatable::Detach();
	Renderable::Detach();
	Tickable::Detach();

	if(mOnComponentRemoved != NULL)
		mOnComponentRemoved->Invoke();
}

void ScriptableComponent::OnEvent(uint32 typeID, uint32 messageID)
{
	if(mOnEvent != NULL)
		mOnEvent->Invoke(typeID, messageID);
}

void ScriptableComponent::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
}

void ScriptableComponent::Update()
{
	if(mUpdate != NULL)
		mUpdate->Invoke();
}

void ScriptableComponent::Tick()
{
	if(mTick != NULL)
		mTick->Invoke();
}

void ScriptableComponent::OnRegistered()
{
	mOnComponentAdded = GetMethod("OnComponentAdded");
	mOnComponentRemoved = GetMethod("OnComponentRemoved");
	mOnEvent = GetMethod("OnEvent");
	mUpdate = GetMethod("Update");
	mTick = GetMethod("Tick");
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

int playstate::Component_FireEvent(lua_State* L)
{
	if(lua_gettop(L) < 3) {
		luaM_printerror(L, "Expected: self<Component>:FireEvent(typeID, messageID)");
		lua_pushnil(L);
		return 1;
	}
		
	uint32 messageID = lua_tointeger(L, -1); lua_pop(L, 1);
	uint32 typeID = lua_tointeger(L, -1); lua_pop(L, 1);
	Component* component = luaM_popobject<Component>(L);
	if(component != NULL) {
		SceneNode* node = component->GetNode();
		if(node != NULL)
			node->FireEvent(typeID, messageID);
		else
			luaM_printerror(L, "Cannot fire an event on a non-attached component");
	} else {
		luaM_printerror(L, "Expected: self<Component>:FireEvent(typeID, messageID)");
	}

	return 0;
}
