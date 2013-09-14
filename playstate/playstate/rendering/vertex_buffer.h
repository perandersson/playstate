#pragma once

#include "../types.h"
#include "primitive_types.h"

namespace playstate
{
	//
	// Container for a vertex buffer.
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() {}

	public:
		//
		// Updates this vertex buffer with new data. The type of vertex data is not allowed to be changed.
		// 
		// @param vertices The data buffer containing all the vertices
		// @param numVertices How many vertices are located in the supplied memory block
		// @throws playstate::RenderingException If the update process fails.
		virtual void Update(const void* vertices, uint32 numVertices) = 0;

		//
		// @return What primitive type this buffer is containing.
		virtual PrimitiveType::Enum GetPrimitiveType() const = 0;
	};
}

