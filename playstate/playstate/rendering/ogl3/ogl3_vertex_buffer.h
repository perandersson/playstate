#pragma once

#include "../vertex_buffer.h"

#include "../exception/rendering_exception.h"
#include "../../math/color.h"
#include "../../math/vector3.h"
#include "../../math/vector2.h"
#include "../vertex_array_object_factory.h"
#include <gl/glew.h>

namespace playstate
{
	class OGL3VertexBuffer : public IVertexBuffer
	{
	public:
		OGL3VertexBuffer(const PrimitiveType& primitiveType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numVertices, uint32 vertexSize);
		~OGL3VertexBuffer();

		void Bind();
		void Render() const;
		void Render(uint32 startIndex) const;
		void Render(uint32 startIndex, uint32 count) const;

		inline GLuint GetBufferID() const {
			return mBufferID;
		}

		inline GLuint GetVertexArrayID() const {
			return mVertexArrayID;
		}

	// IVertexBuffer
	public:
		virtual void Update(const void* vertices, uint32 numVertices);
		virtual const PrimitiveType& GetPrimitiveType() const;

	private:
		GLuint mVertexArrayID;
		GLuint mBufferID;
		const IVertexArrayObjectFactory& mFactory;

	private:
		const PrimitiveType& mPrimitiveType;
		uint32 mNumVertices; 
		uint32 mVertexSize;
	};
}


