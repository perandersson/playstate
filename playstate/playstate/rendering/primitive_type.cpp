#include "../memory/memory.h"
#include "primitive_types.h"
using namespace playstate;

uint32 PrimitiveType::GetElementCount(PrimitiveType::Enum type)
{
	switch(type) {
	case POINT:
		return 1;
	case TRIANGLE:
		return 3;
	case LINE_LOOP:
		return 1;
	}

	return 0;
}
