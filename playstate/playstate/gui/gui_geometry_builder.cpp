#include "../memory/memory.h"
#include "gui_geometry_builder.h"
using namespace playstate;

GuiGeometryBuilder::GuiGeometryBuilder(RenderSystem& renderSystem)
	: mRenderSystem(renderSystem)
{
}

GuiGeometryBuilder::~GuiGeometryBuilder()
{
	mData.clear();
}

void GuiGeometryBuilder::AddQuad(const Rect& rect)
{
	AddQuad(rect, 0);
}

void GuiGeometryBuilder::AddQuad(const Rect& rect, uint32 paletteIndex)
{
	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

	GuiGeometryData elements[6];
	elements[0].Position.Set(rect.X, rect.Y, 0.0f); //p0
	elements[0].PaletteIndex = paletteIndex;
	elements[1].Position.Set(rect.X + rect.Width, rect.Y, 0.0f); //p1
	elements[1].PaletteIndex = paletteIndex;
	elements[2].Position.Set(rect.X, rect.Y + rect.Height, 0.0f); //p2
	elements[2].PaletteIndex = paletteIndex;

	elements[3].Position.Set(rect.X, rect.Y + rect.Height, 0.0f); //p2
	elements[3].PaletteIndex = paletteIndex;
	elements[4].Position.Set(rect.X + rect.Width, rect.Y, 0.0f); //p1
	elements[4].PaletteIndex = paletteIndex;
	elements[5].Position.Set(rect.X + rect.Width, rect.Y + rect.Height, 0.0f); //p3
	elements[5].PaletteIndex = paletteIndex;

	mData.push_back(elements[0]);
	mData.push_back(elements[1]);
	mData.push_back(elements[2]);
	mData.push_back(elements[3]);
	mData.push_back(elements[4]);
	mData.push_back(elements[5]);
}

VertexBuffer* GuiGeometryBuilder::Build()
{
	return mRenderSystem.CreateStaticBuffer(&mData[0], sizeof(GuiGeometryData), GuiGeometryDataVAOFactory, mData.size());
}
