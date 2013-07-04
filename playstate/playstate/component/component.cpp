#include "../memory/memory.h"
#include "component.h"
using namespace playstate;

Component::Component()
	: mOwner(NULL), mTypeMask(0xffffffff), Node(mOwner), TypeMask(mTypeMask)
{
}

Component::Component(uint32 type)
	: mOwner(NULL), mTypeMask(type), Node(mOwner), TypeMask(mTypeMask)
{
}

Component::~Component()
{
	this->OnComponentRemoved();
}

void Component::OnAddedToSceneNode(SceneNode* node)
{
	mOwner = node;
	this->OnComponentAdded();
}

void Component::OnComponentAdded()
{
}

void Component::OnComponentRemoved()
{
}
