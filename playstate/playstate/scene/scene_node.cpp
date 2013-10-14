#include "../memory/memory.h"
#include "scene_node.h"
#include "scene_group.h"
using namespace playstate;

SceneNode::SceneNode()
	: mSceneGroup(NULL), mTypeMask(BIT_ALL), mParent(NULL), 
	mComponents(offsetof(Component, ComponentLink)), mChildren(offsetof(SceneNode, NodeLink)),
	mScale(Vector3::One), mAbsoluteScale(Vector3::One)
{
}

SceneNode::SceneNode(type_mask typeMask)
	: mSceneGroup(NULL), mTypeMask(typeMask), mParent(NULL), 
	mComponents(offsetof(Component, ComponentLink)), mChildren(offsetof(SceneNode, NodeLink)),
	mScale(Vector3::One), mAbsoluteScale(Vector3::One)
{
}

SceneNode::~SceneNode()
{
	mComponents.DeleteAll();
	mChildren.DeleteAll();
	mParent = NULL;
	mSceneGroup = NULL;
}

void SceneNode::AddComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetNode() == NULL && "You are not allowed to add a component on multiple scene nodes");
	
	mComponents.AddLast(component);
	if(IsAttachedToSceneGroup())
		component->OnAttachedToSceneNode(this);
}

void SceneNode::RemoveComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetNode() == this && "You are not allowed to remove a component on that isn't attached here");

	delete component;
}

Component* SceneNode::GetComponent(type_mask typeMask)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		if(BIT_ISSET(component->GetTypeMask(), typeMask))
			return component;

		component = component->ComponentLink.Tail;
	}

	return NULL;
}

void SceneNode::AddChild(SceneNode* node)
{
	assert_not_null(node);

	mChildren.AddLast(node);
	OnChildAdded(node);
}

void SceneNode::RemoveChild(SceneNode* node)
{
	assert_not_null(node);
	assert(node->mParent == this && "You are not allowed to remove another scene objects child node");
	mChildren.Remove(node);
	OnChildRemoved(node);
}

void SceneNode::OnChildAdded(SceneNode* node)
{
	node->OnAttachedToParent(this);

	if(IsAttachedToSceneGroup()) {
		node->NodeAttachedToSceneGroup(mSceneGroup);
	}
}

void SceneNode::OnChildRemoved(SceneNode* node)
{
	node->DetachingNodeFromSceneGroup(mSceneGroup);
	node->OnDetachedFromParent(this);
}

void SceneNode::OnDetachedFromParent(SceneNode* parent)
{
	mParent = NULL;
}

void SceneNode::OnAttachedToParent(SceneNode* parent)
{
	mParent = parent;

	UpdatePosition(mParent->mAbsolutePosition);
	UpdateRotation(mParent->mAbsoluteRotation);
}

void SceneNode::FireEvent(uint32 typeID, uint32 messageID)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->NotifyOnEvent(typeID, messageID);
		component = next;
	}

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->FireEvent(typeID, messageID);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask)
{
	if(!BIT_ISSET(mTypeMask, typeMask))
		return;

	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->NotifyOnEvent(typeID, messageID);
		component = next;
	}

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->FireEvent(typeID, messageID, typeMask);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::SetPosition(const Vector3& position)
{
	Vector3 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdatePosition(mAbsolutePosition);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdatePosition(const Vector3& parentPosition)
{
	mAbsolutePosition = parentPosition + mPosition;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdatePosition(mAbsolutePosition);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::SetRotation(const Vector3& rotation)
{
	Vector3 diff = rotation - mRotation;
	mRotation = rotation;
	mAbsoluteRotation += diff;
	
	UpdateModelMatrix();
	
	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdateRotation(mAbsoluteRotation);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdateRotation(const Vector3& parentRotation)
{
	mAbsoluteRotation = parentRotation + mRotation;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdateRotation(mAbsoluteRotation);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::SetScale(const Vector3& scale)
{
	Vector3 diff = scale - mScale;
	mScale = scale;
	mAbsoluteScale += diff;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdateScale(mAbsoluteScale);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdateScale(const Vector3& parentScale)
{
	mAbsoluteScale = parentScale + mScale;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdateScale(mAbsoluteScale);
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdateModelMatrix()
{
	Vector3 groupPosition;
	if(IsAttachedToSceneGroup()) {
		groupPosition = mSceneGroup->GetPosition();
	}

	mModelMatrix = Matrix4x4::Translation(mAbsolutePosition + groupPosition);
	if(mAbsoluteRotation.IsNotZero())
		mModelMatrix = Matrix4x4::Rotation(mAbsoluteRotation) * mModelMatrix;
	if(mAbsoluteScale.IsNotZero())
		mModelMatrix = Matrix4x4::Scale(mAbsoluteScale) * mModelMatrix;
}

void SceneNode::RemoveFromScene()
{
	if(mSceneGroup != NULL)
		mSceneGroup->RemoveChild(this);
}

void SceneNode::NodeAttachedToSceneGroup(SceneGroup* group)
{
	if(IsAttachedToSceneGroup())
		DetachingNodeFromSceneGroup(group);

	mSceneGroup = group;
	UpdateModelMatrix();
	this->OnAttachedToSceneGroup();
	
	// Nofiy the components that they are attached to the scene via a scene group
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->OnAttachedToSceneNode(this);
		component = next;
	}

	// Attach possible children as well
	SceneNode* child = mChildren.First();
	while(child != NULL) {
		SceneNode* next = child->NodeLink.Tail;
		child->NodeAttachedToSceneGroup(group);
		child = next;
	}
}

void SceneNode::DetachingNodeFromSceneGroup(SceneGroup* group)
{
	if(group == NULL)
		return;

	assert(mSceneGroup == group && "Why are you trying to notify this node that it's being detached from someone elses group?");
	this->OnDetachingFromSceneGroup();
	
	// Detach all this nodes children
	SceneNode* child = mChildren.First();
	while(child != NULL) {
		SceneNode* next = child->NodeLink.Tail;
		child->DetachingNodeFromSceneGroup(group);
		child = next;
	}

	// Nofiy the components that they are being detached from the scene
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->OnDetachingFromSceneNode(this);
		component = next;
	}

	mSceneGroup = NULL;
}

void SceneNode::OnAttachedToSceneGroup()
{
}

void SceneNode::OnDetachingFromSceneGroup()
{
}

namespace playstate
{
	int SceneNode_Factory(lua_State* L)
	{
		int params = lua_gettop(L);
		type_mask typeMask = BIT_ALL;
		if(params == 2) {
			typeMask = (type_mask)lua_tonumber(L, -1); lua_pop(L, 1);
		}

		SceneNode* node = new SceneNode(typeMask);
		luaM_pushobject(L, "SceneNode", node);
		return 1;
	}

	int SceneNode_GetID(lua_State* L)
	{
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL)
			lua_pushnumber(L, node->GetID());
		else
			lua_pushnil(L);

		return 1;
	}

	int SceneNode_AddComponent(lua_State* L)
	{
		Component* component = luaM_popobject<Component>(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(component != NULL && node != NULL) {
			node->AddComponent(component);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:AddComponent(Component)");
		}

		return 0;
	}

	int SceneNode_RemoveComponent(lua_State* L)
	{
		Component* component = luaM_popobject<Component>(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(component != NULL && node != NULL) {
			node->RemoveComponent(component);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:RemoveComponent(Component)");
		}

		return 0;
	}

	int SceneNode_GetPosition(lua_State* L)
	{
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			return luaM_pushvector3(L, node->GetPosition());
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:GetPosition()");
			return luaM_pushvector3(L, Vector3::Zero);
		}
	}

	int SceneNode_SetPosition(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetPosition(vec);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:SetPosition(Vector3)");
		}

		return 0;
	}

	int SceneNode_Translate(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetPosition(node->GetPosition() + vec);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:Translate(Vector3)");
		}

		return 0;
	}
	
	int SceneNode_SetRotatation(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetRotation(vec);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:SetRotatation(Vector3)");
		}

		return 0;
	}
	
	int SceneNode_SetScale(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetScale(vec);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:SetScale(Vector3)");
		}

		return 0;
	}

	int SceneNode_AddChild(lua_State* L)
	{
		SceneNode* child = luaM_popobject<SceneNode>(L);
		SceneNode* parent = luaM_popobject<SceneNode>(L);
		if(child != NULL && parent != NULL) {
			parent->AddChild(child);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:AddNode(SceneNode)");
		}

		return 0;
	}

	int SceneNode_RemoveChild(lua_State* L)
	{
		SceneNode* child = luaM_popobject<SceneNode>(L);
		SceneNode* parent = luaM_popobject<SceneNode>(L);
		if(child != NULL && parent != NULL) {
			parent->RemoveChild(child);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:RemoveNode(SceneNode)");
		}

		return 0;
	}

	int SceneNode_FireEvent(lua_State* L)
	{
		if(lua_gettop(L) < 3) {
			luaM_printerror(L, "Expected: self<SceneNode>:FireEvent(typeID, messageID)");
			lua_pushnil(L);
			return 1;
		}
		
		uint32 messageID = lua_tointeger(L, -1); lua_pop(L, 1);
		uint32 typeID = lua_tointeger(L, -1); lua_pop(L, 1);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->FireEvent(typeID, messageID);
		} else {
			luaM_printerror(L, "Expected: self<SceneNode>:FireEvent(typeID, messageID)");
		}

		return 0;
	}
}
