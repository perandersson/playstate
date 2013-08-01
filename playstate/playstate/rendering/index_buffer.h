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

	private:
		void Render() const;
		void Bind() const;

	private:
		GLuint mIndexBufferId;
		uint32 mNumElements; 
	};
}
