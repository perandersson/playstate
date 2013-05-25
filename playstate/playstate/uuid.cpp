#include "memory/memory.h"
#include "uuid.h"

using namespace playstate;

namespace {
	uint64 _uuid64 = 1;
}

uint32 UUID::To32Bit()
{
	return (uint32)_uuid64++;
}

uint64 UUID::To64Bit()
{
	return _uuid64++;
}
