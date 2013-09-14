#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include <gl/glew.h>

namespace playstate
{
	class VertexBuffer;

	//
	// 
	class IndexBuffer
	{
	public:
		IndexBuffer(GLuint indexBufferId, uint32 numElements);
		~IndexBuffer();

		void Render(VertexBuffer* buffer) const;
		void Render(VertexBuffer* buffer, uint32 firstElement) const;
		void Render(VertexBuffer* buffer, uint32 firstElement, uint32 numElements) const;
		void Bind();

	private:
		GLuint mIndexBufferId;
		uint32 mNumElements; 
	};
}
