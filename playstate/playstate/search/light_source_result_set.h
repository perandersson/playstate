#pragma once

#include "result_set.h"
#include "../processor/light_source.h"

namespace playstate
{

	//
	// 
	class LightSourceResultSet : public ResultSet<LightSource*>
	{
	public:
		LightSourceResultSet();
		~LightSourceResultSet();
	};
}
