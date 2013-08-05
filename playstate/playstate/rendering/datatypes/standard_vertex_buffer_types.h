#pragma once

#include "../../types.h"
#include "../../math/vector3.h"
#include "../../math/vector2.h"
#include "../../math/color.h"
#include "../vertex_declaration.h"

namespace playstate
{
	static const uint32 PositionAttribLocation = 0;
	static const uint32 NormalAttribLocation = 1;
	static const uint32 TexCoordAttribLocation = 2;
	static const uint32 ColorAttribLocation = 3;
	
	//////////////////////////////////////////////////////////

	struct PositionData
	{
		Vector3 Position;
	};
	
	static VertexDeclaration PositionDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionDataVAOFactory(PositionDataVertexDeclaration);

	//////////////////////////////////////////////////////////

	struct PositionTexCoordData
	{
		Vector3 Position;
		Vector2 TexCoord;
	};
	
	static VertexDeclaration PositionTexCoordDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ TexCoordAttribLocation, sizeof(Vector2), 2, GL_FLOAT, false },
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionTexCoordDataVAOFactory(PositionTexCoordDataVertexDeclaration);
	
	//////////////////////////////////////////////////////////

	struct PositionTexCoordColorData
	{
		Vector3 Position;
		Vector2 TexCoord;
		Color Color;
	};
	
	static VertexDeclaration PositionTexCoordColorDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ TexCoordAttribLocation, sizeof(Vector2), 2, GL_FLOAT, false },
			{ ColorAttribLocation, sizeof(Color), 4, GL_FLOAT, false },
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionTexCoordColorDataVAOFactory(PositionTexCoordColorDataVertexDeclaration);
	
	//////////////////////////////////////////////////////////

	struct PositionNormalData
	{
		Vector3 Position;
		Vector3 Normal;
	};
	
	static VertexDeclaration PositionNormalDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ NormalAttribLocation, sizeof(Vector3), 3, GL_FLOAT, true },
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionNormalDataVAOFactory(PositionNormalDataVertexDeclaration);
		
	//////////////////////////////////////////////////////////

	struct PositionNormalTextureData
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoord;
	};

	static VertexDeclaration PositionNormalTextureDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ NormalAttribLocation, sizeof(Vector3), 3, GL_FLOAT, true },
			{ TexCoordAttribLocation, sizeof(Vector2), 2, GL_FLOAT, false },
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionNormalTextureDataVAOFactory(PositionNormalTextureDataVertexDeclaration);
	
	//////////////////////////////////////////////////////////

	struct PositionColorData
	{
		Vector3 Position;
		Color Color;
	};

	static VertexDeclaration PositionColorDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ ColorAttribLocation, sizeof(Color), 4, GL_FLOAT, false },
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory PositionColorDataVAOFactory(PositionColorDataVertexDeclaration);

}