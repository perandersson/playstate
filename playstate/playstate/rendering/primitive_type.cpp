#include "../memory/memory.h"
#include "primitive_types.h"
using namespace playstate;

const playstate::PrimitiveType playstate::PrimitiveType::Point(1, GL_POINTS); 
const playstate::PrimitiveType playstate::PrimitiveType::Triangle(3, GL_TRIANGLES); 
const playstate::PrimitiveType playstate::PrimitiveType::LineLoop(1, GL_LINE_LOOP); 

PrimitiveType::PrimitiveType(uint32 elementCount, GLenum type)
	: mElementCount(elementCount), mType(type)
{
}

PrimitiveType::~PrimitiveType()
{
}

