#include "../memory/memory.h"
#include "component.h"
using namespace playstate;

Component::Component()
	: mOwner(NULL), mType(0), Owner(mOwner), Type(mType)
{
}

Component::Component(uint32 type)
	: mOwner(NULL), mType(type), Owner(mOwner), Type(mType)
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
