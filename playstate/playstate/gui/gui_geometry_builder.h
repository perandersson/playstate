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
		VertexBuffer* VertexBuffer;
		Texture2D* Texture;
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

		void AddQuad(const Vector2& position, const Vector2& size);
		void AddQuad(const Vector2& position, const Vector2& size, const Color& color);
		void AddGradientQuad(const Vector2& position, const Vector2& size, const Color& topColor, const Color& bottomColor);
		void AddGradientQuad(const Vector2& position, const Vector2& size, const Color& topLeftColor, const Color& topRightColor,
			const Color& bottomLeftColor, const Color& bottomRightColor);

		//
		// Adds text into the geometry buffer at the supplied position
		//
		// @param font
		// @param position
		// @param color
		// @param text
		void AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text);
		void AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text, uint32 maxLenght);
		void AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text, const Vector2& maxSize);

		//
		// @return The first building block
		WidgetBuildingBlock* GetBuildingBlocks();

		//
		// @return The number of building blocks built by this geometry builder
		uint32 GetNumBuildingBlocks() const;
		
		void Reset();

	private:
		void AddQuad(const Vector2& position, const Vector2& size, const Color& topLeftColor, const Color& topRightColor,
			const Color& bottomLeftColor, const Color& bottomRightColor, Texture2D* texture);

		void BuildAndPushBuildingBlocks();
		VertexBuffer* PrepareVertexBuffer();

	private:
		IRenderSystem& mRenderSystem;
		VertexBuffer* mVertexBuffer;
		MemoryPool<WidgetGeometryData> mMemoryPool;
		MemoryPool<WidgetBuildingBlock> mBuildingBlocks;

		uint32 mStartIndex;
		uint32 mNumVertices;
		Texture2D* mCurrentTexture;
	};

}
