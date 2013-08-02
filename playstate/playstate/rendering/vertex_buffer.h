#pragma once

#include "../types.h"
#include "exception/rendering_exception.h"
#include "../math/color.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "vertex_array_object_factory.h"
#include <gl/glew.h>

namespace playstate
{
	class GfxProgram;

	const uint32 PositionAttribLocation = 0;
	const uint32 NormalAttribLocation = 1;
	const uint32 TexCoordAttribLocation = 2;
	const uint32 ColorAttribLocation = 3;

	struct PositionData
	{
		Vector3 Position; // always at location = 0
	};

	struct PositionTexCoordData
	{
		Vector3 Position; // always at location = 0
		Vector2 TexCoord; // always at location = 2
	};

	struct PositionNormalData
	{
		Vector3 Position; // always at location = 0
		Vector3 Normal; // always at location = 1
	};

	struct PositionNormalTextureData
	{
		Vector3 Position; // always at location = 0
		Vector3 Normal;  // always at location = 1
		Vector2 TexCoord; // always at location = 2
	};

	// TODO Add struct for color as well - color location = 3.

	//
	// Container for a vertex buffer.
	class VertexBuffer
	{
		friend class GfxProgram;
		
	public:
		VertexBuffer(GLenum vertexType, IVertexArrayObjectFactory& factory, GLuint bufferID, uint32 numElements);
		~VertexBuffer();

	private:
		void Bind();
		void Render() const;
		void Render(uint32 firstElement) const;
		void Render(uint32 firstElement, uint32 numElements) const;

	private:
		GLenum mVertexType;
		GLuint mVertexArrayID;
		GLuint mBufferID;
		uint32 mNumElements;
		IVertexArrayObjectFactory& mFactory;
	};
}

