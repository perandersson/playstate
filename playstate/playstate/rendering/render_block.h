#pragma once

#include "../types.h"
#include "../math/matrix4x4.h"
#include "../math/color.h"

#include <vector>
#include <deque>

namespace playstate
{
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;

	//
	// This block contains all the data neccessary for the pipeline
	// to draw the object onto the screen.
	struct RenderBlock
	{
		uint32 Id;
		VertexBuffer* VertexBuffer;
		IndexBuffer* IndexBuffer;
		Matrix4x4 ModelMatrix;
		Texture2D* DiffuseTexture;
		Color DiffuseColor;
		//Texture2D* AmbientTexture;
		//Texture2D* SpecularTexture;
		//Texture2D* SpecularHighlightTexture;
		//Texture2D* AlphaTexture;
		//Texture2D* BumpMapTexture;
		//Texture2D* DisplacementTexture;
	};
}
