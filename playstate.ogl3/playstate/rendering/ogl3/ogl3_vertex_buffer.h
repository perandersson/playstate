#pragma once

#include <playstate/rendering/vertex_buffer.h>

#include <playstate/rendering/exception/rendering_exception.h>
#include <playstate/rendering/vertex_declaration.h>
#include "ogl3_vertex_declaration_factory.h"
#include <gl/glew.h>

namespace playstate
{
	class OGL3VertexBuffer : public IVertexBuffer
	{
	public:
		OGL3VertexBuffer(PrimitiveType::Enum primitiveType, 
			const VertexDeclaration& vertexDeclaration, GLuint bufferID, uint32 numVertices, uint32 vertexSize);
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
		virtual PrimitiveType::Enum GetPrimitiveType() const;

	private:
		GLuint mVertexArrayID;
		GLuint mBufferID;
		static VertexDeclarationArrayObjectFactory mFactory;
		const VertexDeclaration& mVertexDeclaration;

	private:
		PrimitiveType::Enum mPrimitiveType;
		uint32 mNumVertices; 
		uint32 mVertexSize;
	};
}


