#include "../memory/memory.h"
#include "light_source_result_set.h"
using namespace playstate;

LightSourceResultSet::LightSourceResultSet() 
	: ResultSet<LightSource*>(20, 5)
{
}

LightSourceResultSet::~LightSourceResultSet()
{
}
