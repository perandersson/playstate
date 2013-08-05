#pragma once

#include "../vertex_array_object_factory.h"

namespace playstate
{
	class PositionVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};

	class PositionTexCoordVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};

	class PositionTexCoordColorVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};
	
	class PositionNormalVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};
	
	class PositionColorVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};

	class PositionNormalTextureVAOFactory : public IVertexArrayObjectFactory
	{
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};

}
