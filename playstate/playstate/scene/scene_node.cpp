#include "../memory/memory.h"
#include "scene_node.h"
#include "scene_group.h"
using namespace playstate;

SceneNode::SceneNode()
	: mSceneGroup(NULL), mTypeMask(BIT_ALL), mParent(NULL), 
	mComponents(offsetof(Component, ComponentLink)), mChildren(offsetof(SceneNode, NodeLink))
{
}

SceneNode::SceneNode(type_mask typeMask)
	: mSceneGroup(NULL), mTypeMask(typeMask), mParent(NULL), 
	mComponents(offsetof(Component, ComponentLink)), mChildren(offsetof(SceneNode, NodeLink))
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
		component->OnAttachedToScene(this);
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
		if((component->GetTypeMask() & typeMask) != 0)
			return component;

		component = component->ComponentLink.Tail;
	}

	return NULL;
}

void SceneNode::AddNode(SceneNode* node)
{
	assert_not_null(node);

	mChildren.AddLast(node);
	node->mParent = this;

	node->UpdatePosition();
	node->UpdateRotation();

	if(IsAttachedToSceneGroup()) {
		node->NodeAttachedToSceneGroup(mSceneGroup);
	}
}

void SceneNode::RemoveNode(SceneNode* node)
{
	assert_not_null(node);
	assert(node->mParent == this && "You are not allowed to remove another scene objects child node");
	mChildren.Remove(node);
	node->mParent = NULL;

	node->DetachingNodeFromSceneGroup(mSceneGroup);
}

void SceneNode::SetPosition(const Vector3& position)
{
	Vector3 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdatePosition();
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdatePosition()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsolutePosition = mParent->mAbsolutePosition + mPosition;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdatePosition();
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
		child->UpdateRotation();
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdateRotation()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsoluteRotation = mParent->mAbsoluteRotation + mRotation;

	UpdateModelMatrix();

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdateRotation();
		child = child->NodeLink.Tail;
	}
}

void SceneNode::UpdateModelMatrix()
{
	if(!mAbsoluteRotation.IsZero())
		mModelMatrix = Matrix4x4::Rotation(mAbsoluteRotation) * Matrix4x4::Translation(mAbsolutePosition);
	else
		mModelMatrix = Matrix4x4::Translation(mAbsolutePosition);
}

void SceneNode::RemoveFromScene()
{
	if(mSceneGroup != NULL)
		mSceneGroup->RemoveNode(this);
}

void SceneNode::NodeAttachedToSceneGroup(SceneGroup* group)
{
	if(IsAttachedToSceneGroup())
		DetachingNodeFromSceneGroup(group);

	mSceneGroup = group;
	
	// Nofiy the components that they are attached to the scene via a scene group
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->OnAttachedToScene(this);
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
		component->OnDetachingFromScene(this);
		component = next;
	}

	mSceneGroup = NULL;
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
		}

		return 0;
	}

	int SceneNode_RemoveComponent(lua_State* L)
	{
		Component* component = luaM_popobject<Component>(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(component != NULL && node != NULL) {
			node->RemoveComponent(component);
		}

		return 0;
	}

	int SceneNode_GetPosition(lua_State* L)
	{
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			luaM_pushvector3(L, node->GetPosition());
		} else {
			// Print error
			luaM_pushvector3(L, Vector3::Zero);
		}

		return 3;
	}

	int SceneNode_SetPosition(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetPosition(vec);
		}

		return 0;
	}

	int SceneNode_Translate(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetPosition(node->GetPosition() + vec);
		}

		return 0;
	}
	
	int SceneNode_SetRotatation(lua_State* L)
	{
		Vector3 vec = luaM_popvector3(L);
		SceneNode* node = luaM_popobject<SceneNode>(L);
		if(node != NULL) {
			node->SetRotation(vec);
		}

		return 0;
	}

	int SceneNode_AddNode(lua_State* L)
	{
		SceneNode* child = luaM_popobject<SceneNode>(L);
		SceneNode* parent = luaM_popobject<SceneNode>(L);
		if(child != NULL && parent != NULL) {
			parent->AddNode(child);
		}

		return 0;
	}

	int SceneNode_RemoveNode(lua_State* L)
	{
		SceneNode* child = luaM_popobject<SceneNode>(L);
		SceneNode* parent = luaM_popobject<SceneNode>(L);
		if(child != NULL && parent != NULL) {
			parent->RemoveNode(child);
		}

		return 0;
	}
}
