#pragma once

#include "../types.h"
#include <gl/glew.h>

namespace playstate
{
	class VertexElementType
	{
	public:
		enum Enum {
			BYTE = GL_BYTE,
			UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
			SHORT = GL_SHORT,
			UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
			INT = GL_INT,
			UNSIGNED_INT = GL_UNSIGNED_INT,
			FLOAT = GL_FLOAT,
			DOUBLE = GL_DOUBLE
		};
	};

	//
	struct VertexElementDeclaration
	{
		// The location specifies in what field the specific 
		// element is put when drawing the geometry in the graphics program.
		// This is mapped to the layout location in the vertex shader, for example: {@code layout(location = 0) in vec3 position}
		uint32 Location;

		// The size of the current buffer field type. Use {@code sizeof(Type)}, for example: {@code sizeof(Vector2)}
		uint32 FieldSize;

		// The number of elements in this field. For example a Vector2 contains 2 elements (X and Y).
		uint32 NumElementsInField;

		// What datatype should the element be handled as in the shader. the Vector3 type has float values, which means
		// that this field should be VertexElementType::FLOAT
		VertexElementType::Enum Type;

		// Is the supplied value normalized or not?
		bool Normalized;
	};

	//
	// Structure used to define how vertex buffer data is structured on the graphics card. End the declaration array with a 0 element.
	//
	// You create a vertex declaration like this:
	// {@code
	//  struct PositionTexCoordData
	//	{
	//		Vector3 Position;
	//		Vector2 TexCoord;
	//	};
	//
	//	static const VertexDeclaration PositionTexCoordDataVertexDeclaration = {
	//		{ 
	//			{ 0, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
	//			{ 1, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
	//			0 
	//		}
	//	};
	// }
	struct VertexDeclaration 
	{
		VertexElementDeclaration Elements[8];
	};
}
