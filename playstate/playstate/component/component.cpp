#include "../memory/memory.h"
#include "component.h"
using namespace playstate;

Component::Component()
	: mNode(NULL), mTypeMask(0xffffffff)
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

void Component::OnAddedToSceneNode(SceneNode* node)
{
	mNode = node;
	this->OnComponentAdded();
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
