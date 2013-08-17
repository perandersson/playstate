#pragma once

#include "../types.h"
#include "array_sorter.h"

namespace playstate
{
	//
	// The ResultSet base class is used when performing different types of find queries inside the the game engine.
	template<typename T>
	class IResultSet
	{
	public:
		virtual ~IResultSet() {}
	};
}
