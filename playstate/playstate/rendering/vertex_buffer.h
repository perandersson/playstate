#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include "vertex_array_object_factory.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	//
	// Container for a vertex buffer.
	class VertexBuffer
	{
		friend class GfxProgram;
		
	public:
		VertexBuffer(GLenum vertexType, IVertexArrayObjectFactory* factory, GLuint bufferID, int numElements);
		~VertexBuffer();

	private:
		void Bind();
		void Render() const;

	private:
		GLenum mVertexType;
		GLuint mVertexArrayID;
		GLuint mBufferID;
		uint32 mNumElements;
		IVertexArrayObjectFactory* mFactory;
	};
}

