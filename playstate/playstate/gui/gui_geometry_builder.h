#pragma once

#include "../rendering/render_system.h"
#include "../math/rect.h"
#include <vector>

namespace playstate
{
	struct GuiGeometryData
	{
		Vector3 Position;
		uint32 PaletteIndex;
	};
	
	static VertexDeclaration GuiGeometryDataVertexDeclaration = {
		{ 
			{ 0, sizeof(Vector3), 3, GL_FLOAT, false }, 
			{ 1, sizeof(uint32), 1, GL_UNSIGNED_INT, false }, 
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

		void AddQuad(const Rect& rect);
		void AddQuad(const Rect& rect, uint32 paletteIndex);

		//
		// Creates a vertex buffer based on the supplied geometry
		// 
		// @return 
		VertexBuffer* Build();

	private:
		RenderSystem& mRenderSystem;
		GuiGeometryDatas mData;
	};

}
