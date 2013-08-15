#pragma once

#include "../rendering/render_system.h"
#include "../math/rect.h"
#include <vector>

namespace playstate
{
	struct GuiGeometryData
	{
		Vector2 Position;
		uint32 PaletteIndex;
	};

	static VertexDeclaration GuiGeometryDataVertexDeclaration = {
		{ 
			{ 0, sizeof(Vector2), 2, GL_FLOAT, false }, 
			{ 1, sizeof(uint32), 1, GL_INT, false }, 
			0 
		}
	};
	static VertexDeclarationArrayObjectFactory GuiGeometryDataVAOFactory(GuiGeometryDataVertexDeclaration);

	class GuiGeometryBuilder
	{
		typedef std::vector<GuiGeometryData> GuiGeometryDatas;

	public:
		GuiGeometryBuilder(RenderSystem& renderSystem);
		~GuiGeometryBuilder();

		void AddQuad(const Vector2& position, const Vector2& size);
		void AddQuad(const Vector2& position, const Vector2& size, uint32 paletteIndex);

		//
		// @return The build vertex buffer
		VertexBuffer* GetVertexBuffer();

	private:
		RenderSystem& mRenderSystem;
		VertexBuffer* mVertexBuffer;
		GuiGeometryDatas mData;
	};

}
