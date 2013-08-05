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

	class GuiGeometryDataVAOFactory : public IVertexArrayObjectFactory
	{
		static const uint32 PositionAttribLocation = 0;
		static const uint32 PaletteIndexAttribLocation = 1;

	// IVertexArrayObjectFactory
	public:
		virtual GLuint CreateVertexArray(GLuint bufferId) const;
	};

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
		GuiGeometryDataVAOFactory mGuiGeometryDataVAOFactory;
	};

}
