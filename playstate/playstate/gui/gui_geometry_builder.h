#pragma once

#include "../rendering/render_system.h"
#include "../memory/memory_pool.h"
#include "../font/font.h"
#include "../rendering/texture2d.h"
#include "widget_geometry_declaration.h"

namespace playstate
{
	struct WidgetBuildingBlock
	{
		IVertexBuffer* IVertexBuffer;
		ITexture2D* Texture;
		uint32 StartIndex;
		uint32 NumVertices;
	};

	//
	// The geometry builder is used to append geometry generated by the user interface.
	class GuiGeometryBuilder
	{
	public:
		GuiGeometryBuilder(IRenderSystem& renderSystem);
		~GuiGeometryBuilder();

		//
		// Creates a rectangle onto the screen based on the supplied coordinates and color
		//
		// @param rect
		// @param color
		void AddQuad(const Rect& rect, const Color& color);

		//
		// Creates a rectangle on the screen based on the supplied coordinates and color.
		//
		// @param rect
		// @param topColor
		// @param bottomColor
		void AddGradientQuad(const Rect& rect, const Color& topColor, const Color& bottomColor);

		//
		// Creates a rectangle on the screen based on the supplied coordinates and color.
		//
		// @param rect
		// @param topLeftColor
		// @param topRightColor
		// @param bottomLeftColor
		// @param bottomRightColor
		void AddGradientQuad(const Rect& rect, const Color& topLeftColor, const Color& topRightColor, const Color& bottomLeftColor, const Color& bottomRightColor);
		
		//
		// Adds text into the geometry buffer at the supplied position
		//
		// @param font
		// @param position
		// @param color
		// @param text
		void AddText(Font* font, const Point& position, const Color& color, const playstate::string& text);

		//
		// Adds an aligned text into the geometry buffer at the supplied position
		//
		// @param font
		// @param position
		// @param color
		// @param text
		// @param align
		void AddText(Font* font, const Point& position, const Color& color, const playstate::string& text, FontAlign::Enum align);

		//
		// @return The first building block
		WidgetBuildingBlock* GetBuildingBlocks();

		//
		// @return The number of building blocks built by this geometry builder
		uint32 GetNumBuildingBlocks() const;
		
		void Reset();

	private:
		void AddQuad(const Rect& rect, const Color& topLeftColor, const Color& topRightColor, const Color& bottomLeftColor, const Color& bottomRightColor, ITexture2D* texture);

		void BuildAndPushBuildingBlocks();
		IVertexBuffer* PrepareIVertexBuffer();

	private:
		IRenderSystem& mRenderSystem;
		IVertexBuffer* mIVertexBuffer;
		MemoryPool<WidgetGeometryData> mMemoryPool;
		MemoryPool<WidgetBuildingBlock> mBuildingBlocks;

		uint32 mStartIndex;
		uint32 mNumVertices;
		ITexture2D* mCurrentTexture;
	};

}
