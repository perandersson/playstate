#include "../../memory/memory.h"
#include "ogl3_index_buffer.h"
#include "ogl3_vertex_buffer.h"
using namespace playstate;

OGL3IndexBuffer::OGL3IndexBuffer(GLuint bufferID, uint32 numElements) 
	: mBufferID(bufferID), mNumElements(numElements)
{
}

OGL3IndexBuffer::~OGL3IndexBuffer()
{
	if(mBufferID != 0) {
		glDeleteBuffers(1, &mBufferID);
		mBufferID = 0;
	}
}

void OGL3IndexBuffer::Render(IVertexBuffer* buffer) const
{
	Render(buffer, 0);
}

void OGL3IndexBuffer::Render(IVertexBuffer* buffer, uint32 firstElement) const
{
	Render(buffer, firstElement, mNumElements);
}

void OGL3IndexBuffer::Render(IVertexBuffer* buffer, uint32 firstElement, uint32 numElements) const
{
	// So far only the uint32 indice type is usable
	glDrawElements(buffer->GetPrimitiveType().GetInnerType(), numElements, GL_UNSIGNED_INT, (void*)(firstElement * sizeof(uint32)));
}

void OGL3IndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBufferID);
}
