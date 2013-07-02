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

PositionVAOFactory VertexBuffer::sPositionVAOFactory;
PositionTexCoordVAOFactory VertexBuffer::sPositionTexCoordVAOFactory;
PositionNormalVAOFactory VertexBuffer::sPositionNormalVAOFactory;
PositionNormalTextureVAOFactory VertexBuffer::sPositionNormalTextureVAOFactory;

VertexBuffer::VertexBuffer(GLenum vertexType, IVertexArrayObjectFactory& factory, GLuint bufferID, int numElements) 
	: mVertexType(vertexType), mVertexArrayID(0), mFactory(factory), mBufferID(bufferID), mNumElements(numElements)
{
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
	glDrawArrays(mVertexType, 0, mNumElements);
	
	GLenum error = glGetError();
	if(error != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not draw vertex buffer. Reason: %d", error);
	}
}

VertexBuffer* VertexBuffer::CreateStatic(PositionData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionData), elements, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}
	
	return new VertexBuffer(GL_TRIANGLES, sPositionVAOFactory, bufferID, numElements);
}

VertexBuffer* VertexBuffer::CreateStatic(PositionTexCoordData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionTexCoordData), elements, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, sPositionTexCoordVAOFactory, bufferID, numElements);
}

VertexBuffer* VertexBuffer::CreateStatic(PositionNormalData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionNormalData), elements, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}
	
	return new VertexBuffer(GL_TRIANGLES, sPositionNormalVAOFactory, bufferID, numElements);
}

VertexBuffer* VertexBuffer::CreateStatic(PositionNormalTextureData* elements, uint32 numElements)
{
	GLuint bufferID;
	glGenBuffers(1, &bufferID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, numElements * sizeof(PositionNormalTextureData), elements, GL_STATIC_DRAW);
	glFlush();

	GLenum status = glGetError();
	if(status != GL_NO_ERROR) {
		THROW_EXCEPTION(RenderingException, "Could not create vertex buffer. Reason: %d", status);
	}

	return new VertexBuffer(GL_TRIANGLES, sPositionNormalTextureVAOFactory, bufferID, numElements);
}
