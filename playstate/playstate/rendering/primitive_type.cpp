#include "../memory/memory.h"
#include "primitive_types.h"
#include <gl/glew.h>
using namespace playstate;

const playstate::PrimitiveType playstate::PrimitiveType::Triangle(3, GL_TRIANGLES); 
const playstate::PrimitiveType playstate::PrimitiveType::LineLoop(1, GL_LINE_LOOP); 

PrimitiveType::PrimitiveType(uint32 elementCount, uint32 innerType)
	: mElementCount(elementCount), mInnerType(innerType)
{
}

PrimitiveType::~PrimitiveType()
{
}

