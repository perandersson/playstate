#pragma once

#include <gl/glew.h>

namespace playstate
{
	class IVertexArrayObjectFactory
	{
	public:
		virtual ~IVertexArrayObjectFactory() {}

	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) = 0;
	};
}
