#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include "../math/color.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
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
		VertexBuffer(GLenum vertexType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numElements);
		~VertexBuffer();

	private:
		void Bind();
		void Render() const;
		void Render(uint32 firstElement) const;
		void Render(uint32 firstElement, uint32 numElements) const;

	private:
		GLenum mVertexType;
		GLuint mVertexArrayID;
		GLuint mBufferID;
		uint32 mNumElements;
		const IVertexArrayObjectFactory& mFactory;
	};
}

