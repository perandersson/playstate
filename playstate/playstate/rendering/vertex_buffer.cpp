#include "../memory/memory.h"
#include "vertex_buffer.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/color.h"
using namespace playstate;

#ifdef _DEBUG
#ifndef CHECK_GL_ERROR
#define CHECK_GL_ERROR() \
	{ \
		GLenum err = glGetError(); \
		assert(err == GL_NO_ERROR); \
	}
#endif
#else
#ifndef CHECK_GL_ERROR
#define CHECK_GL_ERROR()
#endif
#endif

#define OFFSET(x) ((char *)NULL + x)

VertexBuffer::VertexBuffer(GLenum vertexType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numElements) 
	: mVertexType(vertexType), mVertexArrayID(0), mFactory(factory), mBufferID(bufferID), mNumElements(numElements)
{
	assert(mVertexType == GL_TRIANGLES && "If this changes then we have to update the render method for vertex buffers");
}

VertexBuffer::~VertexBuffer()
{
	if(mBufferID != 0) {
		glDeleteBuffers(1, &mBufferID);
		mBufferID = 0;
	}

	if(mVertexArrayID != 0) {
		glDeleteVertexArrays(1, &mVertexArrayID);
		mVertexArrayID = 0;
	}
}

void VertexBuffer::Bind()
{
	if(mVertexArrayID == 0)
		mVertexArrayID = mFactory.CreateVertexArray(mBufferID);

	glBindVertexArray(mVertexArrayID);
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not bind vertex buffer. Reason: %d", error);
	}
}

void VertexBuffer::Render() const
{
	Render(0);
}

void VertexBuffer::Render(uint32 firstElement) const
{
	Render(firstElement, mNumElements);
}

void VertexBuffer::Render(uint32 firstElement, uint32 numElements) const
{
	// So far only the GL_TRIANGLES is used as a vertex buffer
	glDrawArrays(mVertexType, firstElement * 3, numElements);
}
