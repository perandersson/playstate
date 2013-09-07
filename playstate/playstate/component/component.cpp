#include "../memory/memory.h"
#include "component.h"
#include "../scene/scene_node.h"
using namespace playstate;

Component::Component()
	: mNode(NULL), mTypeMask(BIT_ALL)
{
}

Component::Component(uint32 type)
	: mNode(NULL), mTypeMask(type)
{
}

Component::~Component()
{
	OnDetachingFromSceneGroup(mGroup);
}

void Component::OnAttachedToSceneNode(SceneNode* node)
{
	assert_not_null(node);
	mNode = node;
	mGroup = node->GetGroup();
	assert_not_null(mGroup);
	this->OnComponentAdded();
}

void Component::OnAttachedToSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	mNode = NULL;
	mGroup = group;
	this->OnComponentAdded();
}

void Component::OnDetachingFromSceneNode(SceneNode* node)
{
	this->OnComponentRemoved();
	mNode = NULL;
	mGroup = NULL;
}

void Component::OnDetachingFromSceneGroup(SceneGroup* group)
{
	this->OnComponentRemoved();
	mNode = NULL;
	mGroup = NULL;
}

void Component::OnComponentAdded()
{
}

void Component::OnComponentRemoved()
{
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

void Component::OnEvent(uint32 typeID, uint32 messageID)
{
}
