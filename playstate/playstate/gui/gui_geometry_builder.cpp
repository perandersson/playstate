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
	AddQuad(position, size, Color::White);
}

void GuiGeometryBuilder::AddQuad(const Vector2& position, const Vector2& size, const Color& color)
{
	AddGradientQuad(position, size, color, color);
}

void GuiGeometryBuilder::AddGradientQuad(const Vector2& position, const Vector2& size, const Color& topColor, const Color& bottomColor)
{
	AddGradientQuad(position, size, topColor, topColor, bottomColor, bottomColor);
}

void GuiGeometryBuilder::AddGradientQuad(const Vector2& position, const Vector2& size, const Color& topLeftColor, const Color& topRightColor,
	const Color& bottomLeftColor, const Color& bottomRightColor)
{
	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

	GuiGeometryData* element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y); //p0
	element->Color = topLeftColor;
	
	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->Color = topRightColor;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->Color = bottomLeftColor;
	
	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->Color = bottomLeftColor;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->Color = topRightColor;

	element = ResultSet<GuiGeometryData>::GetOrCreate();
	element->Position.Set(position.X + size.X, position.Y + size.Y); //p3
	element->Color = bottomRightColor;
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
