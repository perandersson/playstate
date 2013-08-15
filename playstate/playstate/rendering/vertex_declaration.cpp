#include "../memory/memory.h"
#include "vertex_declaration.h"
using namespace playstate;

VertexDeclarationArrayObjectFactory::VertexDeclarationArrayObjectFactory(const VertexDeclaration& declaration)
	: mStride(0), mDeclaration(declaration)
{
	for(uint32 i = 0; i < 8; ++i) {
		const VertexElementDeclaration& elementDecl = declaration.Elements[i];
		if(elementDecl.FieldSize == 0)
			break;

		mStride += elementDecl.FieldSize;
	}
}

VertexDeclarationArrayObjectFactory::~VertexDeclarationArrayObjectFactory()
{
}

GLuint VertexDeclarationArrayObjectFactory::CreateVertexArray(GLuint bufferId) const
{
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	uint32 offset = 0;
	for(uint32 i = 0; i < 8; ++i) {
		const VertexElementDeclaration& elementDecl = mDeclaration.Elements[i];
		if(elementDecl.FieldSize == 0)
			break;
		
		glEnableVertexAttribArray(elementDecl.Location);
		if(HandleAsIntegerType(elementDecl)) {
			glVertexAttribIPointer(elementDecl.Location, elementDecl.NumElementsInField, elementDecl.Type, mStride, OFFSET(offset));
		} else {
			glVertexAttribPointer(elementDecl.Location, elementDecl.NumElementsInField, elementDecl.Type, elementDecl.Normalized ? GL_TRUE : GL_FALSE, mStride, OFFSET(offset));
		}
		offset += elementDecl.FieldSize;
	}
	
	glBindVertexArray(0);
	return vertexArrayID;
}

bool VertexDeclarationArrayObjectFactory::IsIntegerType(const VertexElementDeclaration& declaration) const
{
	const uint32 type = declaration.Type;
	return type == GL_INT || type == GL_UNSIGNED_INT || type == GL_BYTE || type == GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT;
}

bool VertexDeclarationArrayObjectFactory::HandleAsIntegerType(const VertexElementDeclaration& declaration) const
{
	return IsIntegerType(declaration) && !declaration.Normalized;
}
