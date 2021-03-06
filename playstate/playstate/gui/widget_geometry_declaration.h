#pragma once

#include "../math/vector2.h"
#include "../math/color.h"
#include "../rendering/vertex_declaration.h"

namespace playstate
{
	struct WidgetGeometryData
	{
		Vector2 Position;
		Vector2 TexCoord;
		Color Color;
	};

	static const VertexDeclaration WidgetGeometryDataVertexDeclaration = {
		{ 
			{ 0, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			{ 1, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			{ 2, sizeof(Color), 4, VertexElementType::FLOAT, false }, 
			0 
		}
	};
}
