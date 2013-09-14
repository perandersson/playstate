#include "../../memory/memory.h"
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
			glVertexAttribIPointer(elementDecl.Location, elementDecl.NumElementsInField, elementDecl.Type, stride, OFFSET(offset));
		} else {
			glVertexAttribPointer(elementDecl.Location, elementDecl.NumElementsInField, elementDecl.Type, elementDecl.Normalized ? GL_TRUE : GL_FALSE, stride, OFFSET(offset));
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
