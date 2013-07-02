#include "../memory/memory.h"
#include "index_buffer.h"
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

void IndexBuffer::Render() const
{
	glDrawElements(mIndexBufferId, mNumElements, GL_UNSIGNED_INT, NULL);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferId);
}

IndexBuffer* IndexBuffer::CreateStatic(uint32* indices, uint32 numIndices)
{
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(uint32), indices, GL_STATIC_DRAW);
	
	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create index buffer. Reason: %d", status);
	}

	return new IndexBuffer(indexBuffer, numIndices);
}
