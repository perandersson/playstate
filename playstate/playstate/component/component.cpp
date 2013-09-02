#include "../memory/memory.h"
#include "component.h"
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
	this->OnComponentRemoved();
}

void Component::OnAttachedToScene(SceneNode* node)
{
	mNode = node;
	this->OnComponentAdded();
}

void Component::OnDetachingFromScene(SceneNode* node)
{
	this->OnComponentRemoved();
	mNode = NULL;
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
