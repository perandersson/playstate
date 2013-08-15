#pragma once

#include "vertex_array_object_factory.h"
#include "../types.h"

namespace playstate
{
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
		// that this field should be GL_FLOAT
		uint32 Type;

		// Is the supplied value normalized or not?
		bool Normalized;
	};

	//
	// Structure used to define how vertex buffer data is structured on the graphics card. End the declaration array with a 0 element.
	struct VertexDeclaration 
	{
		VertexElementDeclaration Elements[8];
	};

	//
	// VertexArrayObject factory that uses a VertexDeclaration structure to define how the data is ordered and in what location.
	// This should be used as much as possible.
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
	//			{ 0, sizeof(Vector3), 3, GL_FLOAT, false }, 
	//			{ 1, sizeof(Vector2), 2, GL_FLOAT, false },
	//			0 
	//		}
	//	};
	//	static const VertexDeclarationArrayObjectFactory PositionTexCoordDataVAOFactory(PositionTexCoordDataVertexDeclaration);
	// }
	class VertexDeclarationArrayObjectFactory : public IVertexArrayObjectFactory
	{
	public:
		VertexDeclarationArrayObjectFactory(const VertexDeclaration& declaration);
		virtual ~VertexDeclarationArrayObjectFactory();

	// IVertexArrayObjectFactory
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;

	private:
		//
		// @return TRUE if the supplied type is an integer type (byte, short or integer of any variants)
		bool IsIntegerType(const VertexElementDeclaration& declaration) const;

		//
		// @return TRUE if the supplied vertex declaration should be handled as an integer type.
		bool HandleAsIntegerType(const VertexElementDeclaration& declaration) const;

	private:
		uint32 mStride;
		const VertexDeclaration& mDeclaration;
	};
}
