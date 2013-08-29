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
	class StatePolicy;
	class GfxProgram;

	//
	// Container for a vertex buffer.
	class VertexBuffer
	{
		friend class StatePolicy;
		friend class GfxProgram;

	public:
		VertexBuffer(GLenum primitiveType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numVertices, uint32 vertexSize);
		~VertexBuffer();

		//
		// Updates this vertex buffer with new data. The type of vertex data is not allowed to be changed.
		// 
		// @param vertices The data buffer containing all the vertices
		// @param numVertices How many vertices are located in the supplied memory block
		// @throws playstate::RenderingException If the update process fails.
		void Update(const void* vertices, uint32 numVertices);

	private:
		void Bind();
		void Render() const;
		void Render(uint32 startIndex) const;
		void Render(uint32 startIndex, uint32 count) const;

	private:
		GLuint mVertexArrayID;
		GLuint mBufferID;
		const IVertexArrayObjectFactory& mFactory;

	private:
		GLenum mPrimitiveType;
		uint32 mOnePrimitiveCount;
		uint32 mNumVertices; 
		uint32 mVertexSize;
	};
}

