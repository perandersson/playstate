#include "../memory/memory.h"
#include "vertex_buffer.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/color.h"
#include "state/state_policy.h"
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

VertexBuffer::VertexBuffer(GLenum vertexType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numIndices, uint32 elementSize) 
	: mVertexType(vertexType), mVertexArrayID(0), mFactory(factory), mBufferID(bufferID), mNumIndices(numIndices), mElementSize(elementSize)
{
	assert(mVertexType == GL_TRIANGLES && "If this changes then we have to update the render method for vertex buffers");
	assert(elementSize > 0 && "Invalid element size");
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
	// This has to be done during bind time on the main render thread. That's because
	// VertexArrayObjects are not shared between render contexts. (SERIOUSLY DUDES!!!!)
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
	Render(firstElement, mNumIndices);
}

void VertexBuffer::Render(uint32 firstElement, uint32 numIndices) const
{
	static const uint32 numElementsInTriangle = 3;
	glDrawArrays(mVertexType, firstElement * numElementsInTriangle, numIndices);
}

void VertexBuffer::Update(const void* data, uint32 numIndices)
{
	StatePolicy::BindVertexBuffer(this);
	glBindBuffer(GL_ARRAY_BUFFER, mBufferID);

	// Copy the data to the buffer. This can be used if we are simply overridding the existing data.
	glBufferData(GL_ARRAY_BUFFER, numIndices * mElementSize, data, GL_DYNAMIC_DRAW);
	mNumIndices = numIndices;
	glFlush();

	// If we want to update parts of the data - but knows for sure that the size 
	//	shouldn't be changed then this might be what you want.
	// 
	//GLvoid* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	//memcpy(ptr, data, numElements * mElementSize);
	//glUnmapBuffer(GL_ARRAY_BUFFER);

	CHECK_GL_ERROR();
}
