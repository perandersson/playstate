#pragma once
#include "../types.h"

namespace playstate
{
	class PrimitiveType
	{
	public:
		enum Enum {
			POINT,
			TRIANGLE,
			LINE_LOOP,

			NUM_ENUMS
		};

		static uint32 GetElementCount(Enum type);
	};
}
