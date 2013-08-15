#include "../memory/memory.h"
#include "gui_geometry_builder.h"
using namespace playstate;

GuiGeometryBuilder::GuiGeometryBuilder(RenderSystem& renderSystem)
	: mRenderSystem(renderSystem)
{
	mVertexBuffer = mRenderSystem.CreateDynamicBuffer(NULL, sizeof(GuiGeometryData), GuiGeometryDataVAOFactory, 0);
}

GuiGeometryBuilder::~GuiGeometryBuilder()
{
	mData.clear();
	
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
	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

	GuiGeometryData elements[6];
	elements[0].Position.Set(position.X, position.Y); //p0
	elements[0].PaletteIndex = paletteIndex;
	elements[1].Position.Set(position.X + size.X, position.Y); //p1
	elements[1].PaletteIndex = paletteIndex;
	elements[2].Position.Set(position.X, position.Y + size.Y); //p2
	elements[2].PaletteIndex = paletteIndex;

	elements[3].Position.Set(position.X, position.Y + size.Y); //p2
	elements[3].PaletteIndex = paletteIndex;
	elements[4].Position.Set(position.X + size.X, position.Y); //p1
	elements[4].PaletteIndex = paletteIndex;
	elements[5].Position.Set(position.X + size.X, position.Y + size.Y); //p3
	elements[5].PaletteIndex = paletteIndex;

	mData.push_back(elements[0]);
	mData.push_back(elements[1]);
	mData.push_back(elements[2]);
	mData.push_back(elements[3]);
	mData.push_back(elements[4]);
	mData.push_back(elements[5]);
}

VertexBuffer* GuiGeometryBuilder::GetVertexBuffer()
{
	if(!mData.empty()) {
		mVertexBuffer->Update(&mData[0], mData.size());
		mData.clear();
	}

	return mVertexBuffer;
}
