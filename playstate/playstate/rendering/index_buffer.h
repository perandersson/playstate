#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include <gl/glew.h>

namespace playstate
{
	class StatePolicy;
	class GfxProgram;

	//
	// 
	class IndexBuffer
	{
		friend class StatePolicy;
		friend class GfxProgram;

	public:
		IndexBuffer(GLuint indexBufferId, uint32 numElements);
		~IndexBuffer();

	private:
		void Render() const;
		void Render(uint32 firstElement) const;
		void Render(uint32 firstElement, uint32 numElements) const;
		void Bind();

	private:
		GLuint mIndexBufferId;
		uint32 mNumElements; 
	};
}
