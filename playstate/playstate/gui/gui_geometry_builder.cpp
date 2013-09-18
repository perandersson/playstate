#include "../memory/memory.h"
#include "gui_geometry_builder.h"
using namespace playstate;

GuiGeometryBuilder::GuiGeometryBuilder(IRenderSystem& renderSystem)
	: mRenderSystem(renderSystem), mMemoryPool(6, 24), mBuildingBlocks(5, 5), mNumVertices(0), mStartIndex(0), mCurrentTexture(NULL)
{
	mIVertexBuffer = mRenderSystem.CreateDynamicBuffer(NULL, sizeof(WidgetGeometryData), WidgetGeometryDataVertexDeclaration, 0);
}

GuiGeometryBuilder::~GuiGeometryBuilder()
{
	if(mIVertexBuffer != NULL) {
		delete mIVertexBuffer;
		mIVertexBuffer = NULL;
	}
}

void GuiGeometryBuilder::AddQuad(const Rect& rect, const Color& color)
{
	AddGradientQuad(rect, color, color);
}

void GuiGeometryBuilder::AddGradientQuad(const Rect& rect, const Color& topColor, const Color& bottomColor)
{
	AddGradientQuad(rect, topColor, topColor, bottomColor, bottomColor);
}

void GuiGeometryBuilder::AddGradientQuad(const Rect& rect, const Color& topLeftColor, const Color& topRightColor, const Color& bottomLeftColor, 
	const Color& bottomRightColor)
{
	AddQuad(rect, topLeftColor, topRightColor, bottomLeftColor, bottomRightColor, NULL);
}

void GuiGeometryBuilder::AddQuad(const Rect& rect, const Color& topLeftColor, const Color& topRightColor, const Color& bottomLeftColor, 
	const Color& bottomRightColor, ITexture2D* texture)
{
	if(mNumVertices > 0 && mCurrentTexture != texture) {
		BuildAndPushBuildingBlocks();
		mCurrentTexture = texture;
	}

	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

	const Point& position = rect.Position;
	const Size& size = rect.Size;

	WidgetGeometryData* element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y); //p0
	element->Color = topLeftColor;
	
	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.Width, position.Y); //p1
	element->Color = topRightColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y + size.Height); //p2
	element->Color = bottomLeftColor;
	
	element = mMemoryPool.Allocate();
	element->Position.Set(position.X, position.Y + size.Height); //p2
	element->Color = bottomLeftColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.Width, position.Y); //p1
	element->Color = topRightColor;

	element = mMemoryPool.Allocate();
	element->Position.Set(position.X + size.Width, position.Y + size.Height); //p3
	element->Color = bottomRightColor;

	mNumVertices += 6;
}

void GuiGeometryBuilder::AddText(Font* font, const Point& position, const Color& color, const playstate::string& text)
{
	if(mNumVertices > 0 && mCurrentTexture != font->GetTexture()) {
		BuildAndPushBuildingBlocks();
	}
	mCurrentTexture = font->GetTexture();

	Vector2 currentPos(position.X, position.Y);
	bool newline = false;
	const playstate::string::size_type size = text.length();
	for(playstate::string::size_type i = 0; i < size; ++i) {
		playstate::character c = text[i];
		if(c == '\n') {
			newline = true;
			continue;
		} else if(c == '\r') {
			continue;
		} else if(c == ' ') {
			currentPos.X += font->GetSpaceWidth();
			continue;
		}

		const FontCharInfo& info = font->GetFontCharInfo(c);
		if(newline) {
			newline = false;
			currentPos.Y += font->GetLineHeight();
		}

		const Vector2& size = info.Size;
		const Vector2 offsetedPosition = currentPos + Vector2(0, info.Offset.Y);
		
	/*
		p0 ---- p1
		|       |
		p2------p3
	*/

		WidgetGeometryData* element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X, offsetedPosition.Y); //p0
		element->Color = color;
		element->TexCoord.Set(info.TopLeftTexCoord.S, info.TopLeftTexCoord.T);
	
		element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X + size.X, offsetedPosition.Y); //p1
		element->Color = color;
		element->TexCoord.Set(info.BottomRightTexCoord.S, info.TopLeftTexCoord.T);

		element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X, offsetedPosition.Y + size.Y); //p2
		element->Color = color;
		element->TexCoord.Set(info.TopLeftTexCoord.S, info.BottomRightTexCoord.T);
	
		element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X, offsetedPosition.Y + size.Y); //p2
		element->Color = color;
		element->TexCoord.Set(info.TopLeftTexCoord.S, info.BottomRightTexCoord.T);

		element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X + size.X, offsetedPosition.Y); //p1
		element->Color = color;
		element->TexCoord.Set(info.BottomRightTexCoord.S, info.TopLeftTexCoord.T);

		element = mMemoryPool.Allocate();
		element->Position.Set(offsetedPosition.X + size.X, offsetedPosition.Y + size.Y); //p3
		element->Color = color;
		element->TexCoord.Set(info.BottomRightTexCoord.S, info.BottomRightTexCoord.T);

		currentPos.X += info.Size.X;
		mNumVertices += 6;
	}
}

IVertexBuffer* GuiGeometryBuilder::PrepareIVertexBuffer()
{
	const uint32 size = mMemoryPool.GetSize();
	if(size > 0) {
		WidgetGeometryData* data = mMemoryPool.GetFirstElement();
		mIVertexBuffer->Update(data, size);
		mMemoryPool.Reset();
	}

	return mIVertexBuffer;
}

void GuiGeometryBuilder::BuildAndPushBuildingBlocks()
{
	WidgetBuildingBlock* block = mBuildingBlocks.Allocate();
	block->IVertexBuffer = mIVertexBuffer;
	block->Texture = mCurrentTexture;
	block->StartIndex = mStartIndex;
	block->NumVertices = mNumVertices;

	mStartIndex += mNumVertices;
	mNumVertices = 0;
}

WidgetBuildingBlock* GuiGeometryBuilder::GetBuildingBlocks()
{
	if(mNumVertices > 0) {
		BuildAndPushBuildingBlocks();
		PrepareIVertexBuffer();
	}

	return mBuildingBlocks.GetFirstElement();
}

void GuiGeometryBuilder::Reset()
{
	mBuildingBlocks.Reset();
	mStartIndex = 0;
	mNumVertices = 0;
	mCurrentTexture = NULL;
}

uint32 GuiGeometryBuilder::GetNumBuildingBlocks() const
{
	return mBuildingBlocks.GetSize();
}
