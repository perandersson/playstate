#include "../memory/memory.h"
#include "canvas_group.h"
#include "../processor/processors/linked_list_update_processor.h"
#include "../script/script_system.h"
#include "canvas.h"
using namespace playstate;

CanvasGroup::CanvasGroup()
	: mCanvas(NULL), mGeometryBuilder(NULL)
{
}

CanvasGroup::~CanvasGroup()
{
	mCanvas = NULL;
}

void CanvasGroup::AddedToCanvas(Canvas* canvas)
{
	assert(mCanvas == NULL);
	mCanvas = canvas;
	this->OnAddedToCanvas();
}

void CanvasGroup::RemovingFromCanvas(Canvas* canvas)
{
	assert(mCanvas == canvas && "Why are you removing another canvases canvas group?");
	this->OnRemovingFromCanvas();
	mCanvas = NULL;
}

void CanvasGroup::SetStyle(const GuiStyle& style)
{
	mStyle = style;

	mBackColorTop = style.FindColor(SAFE_STRING("BackColor.Top"), Color::HexToRGB("#99"));
	mBackColorBottom = style.FindColor(SAFE_STRING("BackColor.Bottom"), Color::HexToRGB("#22"));
	mFrontColor = style.FindColor(SAFE_STRING("FrontColor"), Color::White);
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
}

void CanvasGroup::ProcessCanvas(GuiGeometryBuilder* builder)
{
	assert_not_null(builder);
	mPositions = std::stack<Vector2>();
	mGeometryBuilder = builder;
	this->OnProcessCanvas();
}

void CanvasGroup::OnProcessCanvas()
{
}

void CanvasGroup::OnAddedToCanvas()
{
}

void CanvasGroup::OnRemovingFromCanvas()
{
	}

Vector2 CanvasGroup::GetAbsolutePosition(const Vector2& relativePosition) const
{
	if(mPositions.empty())
		return relativePosition;

	return mPositions.top() + relativePosition;
}

void CanvasGroup::BeginFrame(const Size& size, const Vector2& position, const playstate::string& title)
{
	const uint32 shadowOffset = 3;
	const uint32 titleHeight = 25;

	static const Color shadowColor(0, 0, 0, 0.3);

	static const Color titleTop = Color::HexToRGB("#EEEEEE");
	static const Color titleBottom = Color::HexToRGB("#777777");

	const Vector2 absolutePosition = GetAbsolutePosition(position);

	// Add shadow
	mGeometryBuilder->AddQuad(absolutePosition - Vector2(shadowOffset, shadowOffset),
		size + Size(shadowOffset * 2, shadowOffset * 2), shadowColor);

	// Add title
	mGeometryBuilder->AddGradientQuad(absolutePosition, Size(size.X, titleHeight), titleTop, titleBottom);

	// Add body
	mGeometryBuilder->AddGradientQuad(absolutePosition + Vector2(0.0f, titleHeight), size - Size(0.0f, titleHeight), mBackColorTop, mBackColorBottom);

	if(!mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), absolutePosition, mFrontColor, title, Size(size.X, titleHeight));
	}

	mPositions.push(absolutePosition);
}

void CanvasGroup::EndFrame()
{
	if(mPositions.size() > 0)
		mPositions.pop();
}

bool CanvasGroup::Button(const Size& size, const Vector2& position, const playstate::string& text)
{
	const uint32 shadowOffset = 3;

	static const Color shadowColor(0, 0, 0, 0.3);
	const Vector2 absolutePosition = GetAbsolutePosition(position);

	// Add shadow
	mGeometryBuilder->AddQuad(absolutePosition - Vector2(shadowOffset, shadowOffset),
		size + Size(shadowOffset * 2, shadowOffset * 2), shadowColor);

	// Add body
	mGeometryBuilder->AddGradientQuad(absolutePosition, size, mBackColorTop, mBackColorBottom);

	if(!mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), absolutePosition, mFrontColor, text, size);
	}

	if(mCanvas->GetMouseClick() == MouseButtons::LEFT) {
		// Check if mouse is over current position
		const Point mousePos = mCanvas->GetMousePosition();
		if(Rect(absolutePosition.X, absolutePosition.Y, size.Width, size.Height).IsPointInside(mousePos)) {
			return true;
		}
	}

	return false;
}

