#include "../memory/memory.h"
#include "collidable.h"
using namespace playstate;

Collidable::Collidable()
	: mCollisionGroups(BIT_NONE)
{
}

Collidable::~Collidable()
{
}

void Collidable::SetCollisionGroups(uint32 groups)
{
	mCollisionGroups = groups;
}

void Collidable::EnableCollisionGroup(uint32 groupID)
{
	BIT_SET(mCollisionGroups, groupID);
}

void Collidable::DisableCollisionGroup(uint32 groupID)
{
	BIT_UNSET(mCollisionGroups, groupID);
}
