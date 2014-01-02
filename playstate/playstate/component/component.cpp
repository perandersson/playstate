#include "../memory/memory.h"
#include "component.h"
#include "../scene/scene_node.h"
#include "../scene/scene_group.h"
using namespace playstate;

Component::Component()
	: Scriptable(), Updatable(), Tickable(), Renderable(), Collidable(),
	mNode(NULL), mTypeMask(BIT_ALL), mFeatures(BIT_NONE),
	mOnComponentAdded(NULL), mOnComponentRemoved(NULL), mOnEvent(NULL), mUpdate(NULL), mTick(NULL)
{
}

Component::Component(uint32 type)
	: Scriptable(), Updatable(), Tickable(), Renderable(), Collidable(),
	mNode(NULL), mTypeMask(type), mFeatures(BIT_NONE),
	mOnComponentAdded(NULL), mOnComponentRemoved(NULL), mOnEvent(NULL), mUpdate(NULL), mTick(NULL)
{
}

Component::~Component()
{
	OnDetachingFromSceneGroup(mGroup);

	delete mOnComponentAdded;
	delete mOnComponentRemoved;
	delete mOnEvent;
	delete mUpdate;
	delete mTick;
}

void Component::OnAttachedToSceneNode(SceneNode* node)
{
	assert_not_null(node);
	mNode = node;
	mGroup = node->GetGroup();
	assert_not_null(mGroup);

	mFeatures = BIT_NONE;
	if(mUpdate != NULL)
		BIT_SET(mFeatures, Features::UPDATABLE);

	if(mTick != NULL)
		BIT_SET(mFeatures, Features::TICKABLE);
	EnableFeatures();

	this->OnComponentAdded();
}

void Component::OnAttachedToSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	mNode = NULL;
	mGroup = group;

	mFeatures = BIT_NONE;
	if(mUpdate != NULL)
		BIT_SET(mFeatures, Features::UPDATABLE);

	if(mTick != NULL)
		BIT_SET(mFeatures, Features::TICKABLE);
	EnableFeatures();

	this->OnComponentAdded();
}

void Component::OnDetachingFromSceneNode(SceneNode* node)
{
	DisableFeatures();
	this->OnComponentRemoved();
	mNode = NULL;
	mGroup = NULL;
}

void Component::OnDetachingFromSceneGroup(SceneGroup* group)
{
	DisableFeatures();
	this->OnComponentRemoved();
	mNode = NULL;
	mGroup = NULL;
}

void Component::EnableFeatures()
{
	if(BIT_ISSET(mFeatures, Features::UPDATABLE)) {
		GetGroup()->AttachUpdatable(this);
	}

	if(BIT_ISSET(mFeatures, Features::TICKABLE)) {
		GetGroup()->AttachTickable(this);
	}

	if(BIT_ISSET(mFeatures, Features::RENDERABLE)) {
		GetNode()->AttachRenderable(this);
	}

	if(BIT_ISSET(mFeatures, Features::COLLIDABLE)) {
	}
}

void Component::DisableFeatures()
{
	if(BIT_ISSET(mFeatures, Features::UPDATABLE)) {
		GetGroup()->DetachUpdatable(this);
	}

	if(BIT_ISSET(mFeatures, Features::TICKABLE)) {
		GetGroup()->DetachTickable(this);
	}

	if(BIT_ISSET(mFeatures, Features::RENDERABLE)) {
		GetNode()->DetachRenderable(this);
	}

	if(BIT_ISSET(mFeatures, Features::COLLIDABLE)) {
	}
}

void Component::OnComponentAdded()
{
	if(mOnComponentAdded != NULL)
		mOnComponentAdded->Invoke();
}

void Component::OnComponentRemoved()
{
	if(mOnComponentRemoved != NULL)
		mOnComponentRemoved->Invoke();
}

SceneNode* Component::GetNode() const
{
	return mNode;
}

SceneGroup* Component::GetGroup() const
{
	return mGroup;
}

type_mask Component::GetTypeMask() const
{
	return mTypeMask;
}

void Component::SetTypeMask(type_mask typeMask)
{
	mTypeMask = typeMask;
}

void Component::NotifyOnEvent(uint32 typeID, uint32 messageID)
{
	this->OnEvent(typeID, messageID);
}

void Component::SetFeatures(type_mask features)
{
	const type_mask disableFeatures = ~mFeatures ^ ~features;
	if(BIT_ISSET(disableFeatures, Features::UPDATABLE)) {
		GetGroup()->DetachUpdatable(this);
	}

	if(BIT_ISSET(disableFeatures, Features::TICKABLE)) {
		GetGroup()->DetachTickable(this);
	}

	if(BIT_ISSET(disableFeatures, Features::RENDERABLE)) {
		GetNode()->DetachRenderable(this);
	}

	if(BIT_ISSET(disableFeatures, Features::COLLIDABLE)) {
	}

	const type_mask enableFeatures = mFeatures ^ features;
	if(BIT_ISSET(enableFeatures, Features::UPDATABLE)) {
		GetGroup()->AttachUpdatable(this);
	}

	if(BIT_ISSET(enableFeatures, Features::TICKABLE)) {
		GetGroup()->AttachTickable(this);
	}

	if(BIT_ISSET(enableFeatures, Features::RENDERABLE)) {
		GetNode()->AttachRenderable(this);
	}

	if(BIT_ISSET(enableFeatures, Features::COLLIDABLE)) {
	}

	mFeatures = features;
}

void Component::EnableFeature(type_mask feature)
{
	if(BIT_ISSET(feature, Features::UPDATABLE)) {
		GetGroup()->AttachUpdatable(this);

	}

	if(BIT_ISSET(feature, Features::TICKABLE)) {
		GetGroup()->AttachTickable(this);
	}

	if(BIT_ISSET(feature, Features::RENDERABLE)) {
		GetNode()->AttachRenderable(this);
	}

	if(BIT_ISSET(feature, Features::COLLIDABLE)) {

	}

	BIT_SET(mFeatures, feature);
}

void Component::DisableFeature(type_mask feature)
{
	if(BIT_ISSET(feature, Features::UPDATABLE)) {
		GetGroup()->DetachUpdatable(this);
	}

	if(BIT_ISSET(feature, Features::TICKABLE)) {
		GetGroup()->DetachTickable(this);
	}

	if(BIT_ISSET(feature, Features::RENDERABLE)) {
		GetNode()->DetachRenderable(this);
	}

	if(BIT_ISSET(feature, Features::COLLIDABLE)) {

	}

	BIT_UNSET(mFeatures, feature);
}

void Component::OnEvent(uint32 typeID, uint32 messageID)
{
	if(mOnEvent != NULL)
		mOnEvent->Invoke(typeID, messageID);
}

void Component::Update()
{
	if(mUpdate != NULL)
		mUpdate->Invoke();
}

void Component::Tick()
{
	if(mTick != NULL)
		mTick->Invoke();
}

void Component::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
}

void Component::CollidesWith(ICollidable* otherObject, const Vector3& direction)
{
}

void Component::OnRegistered()
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

	Component* node = new Component(typeMask);
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
		
	Component* component = luaM_popobject<Component>(L);
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
	Component* component = luaM_popobject<Component>(L);
	if(component != NULL) {
		SceneNode* owner = component->GetNode();
		owner->SetPosition(owner->GetPosition() + vec);
	}

	return 0;
}

int playstate::Component_SetNodePosition(lua_State* L)
{
	Vector3 vec = luaM_popvector3(L);
	Component* component = luaM_popobject<Component>(L);
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
	Component* component = luaM_popobject<Component>(L);
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
		
	Component* component = luaM_popobject<Component>(L);
	if(component != NULL) {
		luaM_pushvector3(L, component->GetNode()->GetPosition());
	} else {
		luaM_pushvector3(L, Vector3::Zero);
	}
		
	return 3;
}
//
//int playstate::Component_Show(lua_State* L)
//{
//	if(lua_gettop(L) < 1) {
//		luaM_printerror(L, "Expected: self<Component>:Show()");
//		return 0;
//	}
//
//	Component* component = luaM_popobject<Component>(L);
//	if(component != NULL) {
//		component->Show();
//	}
//
//	return 0;
//}
//
//int playstate::Component_Hide(lua_State* L)
//{
//	if(lua_gettop(L) < 1) {
//		luaM_printerror(L, "Expected: self<Component>:Hide()");
//		return 0;
//	}
//
//	Component* component = luaM_popobject<Component>(L);
//	if(component != NULL) {
//		component->Hide();
//	}
//
//	return 0;
//}

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
