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
