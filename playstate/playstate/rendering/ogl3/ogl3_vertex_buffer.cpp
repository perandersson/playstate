#include "../../memory/memory.h"
#include "ogl3_vertex_buffer.h"
#include "../../math/vector3.h"
#include "../../math/vector2.h"
#include "../../math/color.h"
#include "ogl3_state_policy.h"
using namespace playstate;

OGL3VertexBuffer::OGL3VertexBuffer(const PrimitiveType& primitiveType, const IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numVertices, uint32 vertexSize) 
	: mPrimitiveType(primitiveType), mVertexArrayID(0), mFactory(factory), mBufferID(bufferID), mNumVertices(numVertices), mVertexSize(vertexSize)
{
	assert(mVertexSize > 0 && "The size of one vertex cannot be 0");
}

OGL3VertexBuffer::~OGL3VertexBuffer()
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

void OGL3VertexBuffer::Bind()
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

void OGL3VertexBuffer::Render() const
{
	Render(0);
}

void OGL3VertexBuffer::Render(uint32 startIndex) const
{
	Render(startIndex, mNumVertices);
}

void OGL3VertexBuffer::Render(uint32 startIndex, uint32 count) const
{
	// Prevent you from drawing more vertices than exist in the buffer
	if(count > mNumVertices - startIndex) {
		count = mNumVertices - startIndex;
	}

	glDrawArrays(mPrimitiveType.GetInnerType(), startIndex, count);
}

void OGL3VertexBuffer::Update(const void* vertices, uint32 numVertices)
{
	OGL3StatePolicyGuard::BindVertexBuffer(this);
	glBindBuffer(GL_ARRAY_BUFFER, mBufferID);

	// Copy the data to the buffer. This can be used if we are simply overridding the existing data.
	glBufferData(GL_ARRAY_BUFFER, numVertices * mVertexSize, vertices, GL_DYNAMIC_DRAW);
	mNumVertices = numVertices;
	glFlush();

	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		THROW_EXCEPTION(RenderingException, "Could not update vertex buffer with new data. Reason: '%d'", error);
}

const PrimitiveType& OGL3VertexBuffer::GetPrimitiveType() const
{
	return mPrimitiveType;
}
