#pragma once

#include "../types.h"

namespace playstate
{
	class VertexBuffer;
	class IndexBuffer;
	//class Texture2D;

	struct GuiRenderBlock
	{
		uint32 Id;
		VertexBuffer* VertexBuffer;
		//Font* Font;
		uint32 Depth;
	};
		
}
