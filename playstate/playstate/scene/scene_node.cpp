#include "../memory/memory.h"
#include "scene_node.h"
#include "scene_group.h"
using namespace playstate;

SceneNode::SceneNode(SceneGroup* group)
	: mSceneGroup(group), mTypeMask(0xffffffff), mParent(NULL),
	Rotation(mRotation), AbsoluteRotation(mAbsoluteRotation),
	Position(mPosition), AbsolutePosition(mAbsolutePosition),
	ModelMatrix(mModelMatrix), Group(mSceneGroup), TypeMask(mTypeMask)
{
	assert_not_null(group);
	group->AddSceneNode(this);
}


SceneNode::SceneNode(SceneGroup* group, type_mask typeMask)
	: mSceneGroup(group), mTypeMask(typeMask), mParent(NULL),
	Rotation(mRotation), AbsoluteRotation(mAbsoluteRotation),
	Position(mPosition), AbsolutePosition(mAbsolutePosition),
	ModelMatrix(mModelMatrix), Group(mSceneGroup), TypeMask(mTypeMask)
{
	assert_not_null(group);
	group->AddSceneNode(this);
}

SceneNode::~SceneNode()
{
	mComponents.DeleteAll();

	ChildNodes::iterator it = mChildren.begin();
	while(it != mChildren.end()) {
		SceneNode* node = *it;
		it = it++;
		delete node;
	}

	if(mParent != NULL) {
		mParent->RemoveChildNode(this);
	}
	
	mSceneGroup = NULL;
}

void SceneNode::AddComponent(Component* component)
{
	assert_not_null(component);
	assert(component->Node == NULL && "You are not allowed to add a component on multiple scene nodes");
	
	mComponents.AddLast(component);
	component->OnAddedToSceneNode(this);
}

void SceneNode::RemoveComponent(Component* component)
{
	assert_not_null(component);
	assert(component->Node == this && "You are not allowed to remove a component on that isn't attached here");

	delete component;
}

Component* SceneNode::GetComponent(type_mask typeMask)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		if((component->TypeMask & typeMask) != 0)
			return component;

		component = component->ComponentLink.Tail;
	}

	return NULL;
}

void SceneNode::AddChildNode(SceneNode* node)
{
	assert_not_null(node);

	mChildren.push_back(node);
	node->mParent = this;

	node->UpdatePosition();
	node->UpdateRotation();
}

void SceneNode::RemoveChildNode(SceneNode* node)
{
	assert_not_null(node);
	assert(node->mParent == this && "You are not allowed to remove another scene objects child node");

	ChildNodes::iterator it = std::find(mChildren.begin(), mChildren.end(), node);
	if(it != mChildren.end()) {
		mChildren.erase(it);
		node->mParent = NULL;
	}
}

void SceneNode::SetPosition(const Vector3& position)
{
	Vector3 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;

	UpdateModelMatrix();

	ChildNodes::size_type size = mChildren.size();
	for(ChildNodes::size_type i = 0; i < size; ++i) {
		mChildren[i]->UpdatePosition();
	}
}

void SceneNode::UpdatePosition()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsolutePosition = mParent->mAbsolutePosition + mPosition;

	UpdateModelMatrix();
}

void SceneNode::SetRotation(const Vector3& rotation)
{
	Vector3 diff = rotation - mRotation;
	mRotation = rotation;
	mAbsoluteRotation += diff;
	
	UpdateModelMatrix();
	
	ChildNodes::size_type size = mChildren.size();
	for(ChildNodes::size_type i = 0; i < size; ++i) {
		mChildren[i]->UpdateRotation();
	}
}

void SceneNode::UpdateRotation()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsoluteRotation = mParent->mAbsoluteRotation + mRotation;
	UpdateModelMatrix();
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
	assert(mSceneGroup != NULL && "You cannot remove a scene node that isn't owned by a scene");
	mSceneGroup->RemoveSceneNode(this);
}

namespace playstate
{
	
	int SceneNode_Factory(lua_State* L)
	{
		SceneGroup* group = luaM_popobject<SceneGroup>(L);
		if(group != NULL) {
			SceneNode* node = new SceneNode(group);
			luaM_pushobject(L, "SceneNode", node);
		} else {
			lua_pushnil(L);
		}
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
			float* arr = (float*)lua_newuserdata(L, sizeof(float[3]));
			arr[0] = node->Position.X;
			arr[1] = node->Position.Y;
			arr[2] = node->Position.Z;
		} else {
			lua_pushnil(L);
		}

		return 1;
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
			node->SetPosition(node->Position + vec);
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
}
