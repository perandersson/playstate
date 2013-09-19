#pragma once

#include "../types.h"

namespace playstate
{
	//
	// Defines the font alignment
	class FontAlign
	{
	public:
		enum Enum {
			LEFT,
			CENTER,
			RIGHT
		};

		static const Enum Default = LEFT;
	};
}
