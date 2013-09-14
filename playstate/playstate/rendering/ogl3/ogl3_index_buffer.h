#pragma once

#include "../index_buffer.h"
#include <gl/glew.h>

namespace playstate
{
	class IVertexBuffer;

	//
	// 
	class OGL3IndexBuffer : public IIndexBuffer
	{
	public:
		OGL3IndexBuffer(GLuint bufferID, uint32 numElements);
		virtual ~OGL3IndexBuffer();

		void Render(IVertexBuffer* buffer) const;
		void Render(IVertexBuffer* buffer, uint32 firstElement) const;
		void Render(IVertexBuffer* buffer, uint32 firstElement, uint32 numElements) const;
		void Bind();

	private:
		GLuint mBufferID;
		uint32 mNumElements; 
	};
}
