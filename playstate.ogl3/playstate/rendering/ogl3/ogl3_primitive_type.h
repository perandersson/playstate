#pragma once

#include <playstate/rendering/primitive_types.h>
#include <gl/glew.h>

namespace playstate
{
	static const GLenum OGL3PrimitiveType[PrimitiveType::NUM_ENUMS] = {
		GL_POINTS,
		GL_TRIANGLES,
		GL_LINE_LOOP
	};
}
