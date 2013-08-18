#include "../memory/memory.h"
#include "gui_geometry_builder.h"
using namespace playstate;

GuiGeometryBuilder::GuiGeometryBuilder(RenderSystem& renderSystem)
	: mRenderSystem(renderSystem), mMemoryPool(6, 24)
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

	GuiGeometryData* element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y); //p0
	element->Color = topLeftColor;
	
	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->Color = topRightColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->Color = bottomLeftColor;
	
	element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y + size.Y); //p2
	element->Color = bottomLeftColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.X, position.Y); //p1
	element->Color = topRightColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.X, position.Y + size.Y); //p3
	element->Color = bottomRightColor;
}

void GuiGeometryBuilder::AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text)
{
	Vector2 currentPos = position;
	bool newline = false;
	playstate::string::size_type size = text.length();
	for(playstate::string::size_type i = 0; i < size; ++i) {
		playstate::character c = text[i];
		if(c == '\n') {
			newline = true;
			continue;
		} else if(c == '\r') {
			continue;
		}

		const FontCharInfo& info = font->GetFontCharInfo(c);
		if(newline) {
			newline = false;
			currentPos.Y += info.Size.Y;
		}
		AddQuad(currentPos, info.Size, Color::Black);
		currentPos.X += info.Size.X;
	}
}

void GuiGeometryBuilder::AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text, uint32 maxLenght)
{
}

void GuiGeometryBuilder::AddText(Font* font, const Vector2& position, const Color& color, const playstate::string& text, const Vector2& maxSize)
{
}

VertexBuffer* GuiGeometryBuilder::GetVertexBuffer()
{
	const uint32 size = mMemoryPool.GetSize();
	if(size > 0) {
		GuiGeometryData* data = mMemoryPool.GetFirstElement();
		mVertexBuffer->Update(data, size);
		mMemoryPool.Reset();
	}

	return mVertexBuffer;
}
