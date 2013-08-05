#include "../../memory/memory.h"
#include "vertex_array_object_factories.h"
#include "../render_system.h"
using namespace playstate;

GLuint PositionVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const uint32 stride = sizeof(PositionData);
	
	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);
	
	glBindVertexArray(0);
	return vertexArrayID;
}

GLuint PositionTexCoordVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const uint32 stride = sizeof(PositionTexCoordData);
	
	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);
	
	glEnableVertexAttribArray(TexCoordAttribLocation);
	glVertexAttribPointer(TexCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(sizeof(Vector3)));
	
	glBindVertexArray(0);
	return vertexArrayID;
}

GLuint PositionTexCoordColorVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const uint32 stride = sizeof(PositionTexCoordColorData);
	
	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);
	
	glEnableVertexAttribArray(TexCoordAttribLocation);
	glVertexAttribPointer(TexCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(sizeof(Vector3)));
	
	glEnableVertexAttribArray(ColorAttribLocation);
	glVertexAttribPointer(ColorAttribLocation, 4, GL_FLOAT, GL_FALSE, stride, OFFSET(sizeof(Vector3) + sizeof(Vector2)));

	glBindVertexArray(0);
	return vertexArrayID;
}

GLuint PositionNormalVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const uint32 stride = sizeof(PositionNormalData);
	
	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(NormalAttribLocation);
	glVertexAttribPointer(NormalAttribLocation, 3, GL_FLOAT, GL_TRUE, stride, OFFSET(sizeof(Vector3)));
	
	glBindVertexArray(0);
	return vertexArrayID;
}


GLuint PositionColorVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	const uint32 stride = sizeof(PositionColorData);
	
	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(ColorAttribLocation);
	glVertexAttribPointer(ColorAttribLocation, 4, GL_FLOAT, GL_FALSE, stride, OFFSET(sizeof(Vector3)));
	
	glBindVertexArray(0);
	return vertexArrayID;
}

GLuint PositionNormalTextureVAOFactory::CreateVertexArray(GLuint bufferID) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	
	const uint32 stride = sizeof(PositionNormalTextureData);

	glEnableVertexAttribArray(PositionAttribLocation);
	glVertexAttribPointer(PositionAttribLocation, 3, GL_FLOAT, GL_FALSE, stride, 0);

	glEnableVertexAttribArray(NormalAttribLocation);
	glVertexAttribPointer(NormalAttribLocation, 3, GL_FLOAT, GL_TRUE, stride, OFFSET(sizeof(Vector3)));
	
	glEnableVertexAttribArray(TexCoordAttribLocation);
	glVertexAttribPointer(TexCoordAttribLocation, 2, GL_FLOAT, GL_FALSE, stride, OFFSET(sizeof(Vector3) + sizeof(Vector3)));
	
	glBindVertexArray(0);
	return vertexArrayID;
}
