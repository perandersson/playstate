#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	//
	// 
	class IndexBuffer
	{
		friend class GfxProgram;

	public:
		IndexBuffer(GLuint indexBufferId, uint32 numElements);
		~IndexBuffer();

		//
		// Creates an index buffer based on a supplied of indices
		// @return A bindable and renderable index buffer
		// @throws RenderingException
		static IndexBuffer* CreateStatic(uint32* indices, uint32 numIndices);

	private:
		void Render() const;
		void Bind() const;

	private:
		GLuint mIndexBufferId;
		uint32 mNumElements; 
	};
}
