#pragma once

#include "../vertex_array_object_factory.h"

namespace playstate
{
	class PositionVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId);
	};

	class PositionTexCoordVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId);
	};
	
	class PositionNormalVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId);
	};
	
	class PositionNormalTextureVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId);
	};
}
