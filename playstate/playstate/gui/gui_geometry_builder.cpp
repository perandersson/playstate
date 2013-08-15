#include "../memory/memory.h"
#include "gui_geometry_builder.h"
using namespace playstate;

GuiGeometryBuilder::GuiGeometryBuilder(RenderSystem& renderSystem)
	: mRenderSystem(renderSystem), ResultSet<GuiGeometryData>(6, 24)
{
	mVertexBuffer = mRenderSystem.CreateDynamicBuffer(NULL, sizeof(GuiGeometryData), GuiGeometryDataVAOFactory, 0);
}

GuiGeometryBuilder::~GuiGeometryBuilder()
{
	if(mVertexBuffer != NULL) {
		delete mVertexBuffer;
		mVertexBuffer = NULL;
	}
}

void GuiGeometryBuilder::AddQuad(const Vector2& position, const Vector2& size)
{
	AddQuad(position, size, 0);
}

void GuiGeometryBuilder::AddQuad(const Vector2& position, const Vector2& size, uint32 paletteIndex)
{
	AddGradientQuad(position, size, paletteIndex, paletteIndex, paletteIndex, paletteIndex);
}

void GuiGeometryBuilder::AddGradientQuad(const Vector2& position, const Vector2& size, uint32 topLeftPaletteIndex, uint32 topRightPaletteIndex,
	uint32 bottomLeftPaletteIndex, uint32 bottomRightPaletteIndex)
{
	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

	GuiGeometryData* element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y); //p0
	element->PaletteIndex = topLeftPaletteIndex;
	
	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->PaletteIndex = topRightPaletteIndex;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->PaletteIndex = bottomLeftPaletteIndex;
	
	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->PaletteIndex = bottomLeftPaletteIndex;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->PaletteIndex = topRightPaletteIndex;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y + size.Y); //p3
	element->PaletteIndex = bottomRightPaletteIndex;
}

VertexBuffer* GuiGeometryBuilder::GetVertexBuffer()
{
	if(ResultSet<GuiGeometryData>::GetSize() > 0) {
		GuiGeometryData* elements = ResultSet<GuiGeometryData>::GetElements();
		mVertexBuffer->Update(elements, ResultSet<GuiGeometryData>::GetSize());
		ResultSet<GuiGeometryData>::Reset();
	}

	return mVertexBuffer;
}
