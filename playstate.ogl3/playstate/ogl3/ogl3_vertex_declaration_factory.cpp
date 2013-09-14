#include <playstate/memory/memory.h>
#include "ogl3_vertex_declaration_factory.h"
using namespace playstate;

VertexDeclarationArrayObjectFactory::VertexDeclarationArrayObjectFactory()
{
	
}

VertexDeclarationArrayObjectFactory::~VertexDeclarationArrayObjectFactory()
{
}

GLuint VertexDeclarationArrayObjectFactory::CreateVertexArray(const VertexDeclaration& declaration, GLuint bufferId) const
{
	uint32 stride = 0;
	for(uint32 i = 0; i < 8; ++i) {
		const VertexElementDeclaration& elementDecl = declaration.Elements[i];
		if(elementDecl.FieldSize == 0)
			break;

		stride += elementDecl.FieldSize;
	}

	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	glBindBuffer(GL_ARRAY_BUFFER, bufferId);

	uint32 offset = 0;
	for(uint32 i = 0; i < 8; ++i) {
		const VertexElementDeclaration& elementDecl = declaration.Elements[i];
		if(elementDecl.FieldSize == 0)
			break;
		
		glEnableVertexAttribArray(elementDecl.Location);
		if(HandleAsIntegerType(elementDecl)) {
			glVertexAttribIPointer(elementDecl.Location, elementDecl.NumElementsInField, OGL3VertexElementType[elementDecl.Type], stride, OFFSET(offset));
		} else {
			glVertexAttribPointer(elementDecl.Location, elementDecl.NumElementsInField, OGL3VertexElementType[elementDecl.Type], elementDecl.Normalized ? GL_TRUE : GL_FALSE, stride, OFFSET(offset));
		}
		offset += elementDecl.FieldSize;
	}
	
	glBindVertexArray(0);
	return vertexArrayID;
}

bool VertexDeclarationArrayObjectFactory::IsIntegerType(const VertexElementDeclaration& declaration) const
{
	const VertexElementType::Enum type = declaration.Type;
	return type == VertexElementType::INT || type == VertexElementType::UNSIGNED_INT || type == VertexElementType::BYTE 
		|| type == VertexElementType::UNSIGNED_BYTE || type == VertexElementType::SHORT || type == VertexElementType::UNSIGNED_SHORT;
}

bool VertexDeclarationArrayObjectFactory::HandleAsIntegerType(const VertexElementDeclaration& declaration) const
{
	return IsIntegerType(declaration) && !declaration.Normalized;
}
