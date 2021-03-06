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
	
	static const VertexDeclaration PositionDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			0 
		}
	};

	//////////////////////////////////////////////////////////

	struct PositionTexCoordData
	{
		Vector3 Position;
		Vector2 TexCoord;
	};
	
	static const VertexDeclaration PositionTexCoordDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ TexCoordAttribLocation, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			0 
		}
	};
	
	//////////////////////////////////////////////////////////

	struct PositionTexCoordColorData
	{
		Vector3 Position;
		Vector2 TexCoord;
		Color Color;
	};
	
	static const VertexDeclaration PositionTexCoordColorDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ TexCoordAttribLocation, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			{ ColorAttribLocation, sizeof(Color), 4, VertexElementType::FLOAT, false },
			0 
		}
	};
	
	//////////////////////////////////////////////////////////

	struct PositionNormalData
	{
		Vector3 Position;
		Vector3 Normal;
	};
	
	static const VertexDeclaration PositionNormalDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ NormalAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, true },
			0 
		}
	};
		
	//////////////////////////////////////////////////////////

	struct PositionNormalTextureData
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoord;
	};

	static const VertexDeclaration PositionNormalTextureDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ NormalAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, true },
			{ TexCoordAttribLocation, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			0 
		}
	};
	
	//////////////////////////////////////////////////////////

	struct PositionColorData
	{
		Vector3 Position;
		Color Color;
	};

	static const VertexDeclaration PositionColorDataVertexDeclaration = {
		{ 
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ ColorAttribLocation, sizeof(Color), 4, VertexElementType::FLOAT, false },
			0 
		}
	};

}
