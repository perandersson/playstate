#include "../memory/memory.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
using namespace playstate;

IndexBuffer::IndexBuffer(GLuint indexBufferId, uint32 numElements) 
	: mIndexBufferId(indexBufferId), mNumElements(numElements)
{
}

IndexBuffer::~IndexBuffer()
{
	if(mIndexBufferId != 0) {
		glDeleteBuffers(1, &mIndexBufferId);
		mIndexBufferId = 0;
	}
}

void IndexBuffer::Render(VertexBuffer* buffer) const
{
	Render(buffer, 0);
}

void IndexBuffer::Render(VertexBuffer* buffer, uint32 firstElement) const
{
	Render(buffer, firstElement, mNumElements);
}

void IndexBuffer::Render(VertexBuffer* buffer, uint32 firstElement, uint32 numElements) const
{
	// So far only the uint32 indice type is usable
	glDrawElements(buffer->GetPrimitiveType(), numElements, GL_UNSIGNED_INT, (void*)(firstElement * sizeof(uint32)));
}

void IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
}
