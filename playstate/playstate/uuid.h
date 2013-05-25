#pragma once

#include "types.h"

namespace playstate
{
	class UUID
	{
	public:
		static uint32 To32Bit();
		static uint64 To64Bit();
	};
}

