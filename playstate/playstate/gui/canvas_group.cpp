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

	mShadowOffset = style.FindInt(SAFE_STRING("ShadowOffset"), 0);
	mShadowColor = style.FindColor(SAFE_STRING("ShadowColor"), Color(0, 0, 0, 0.3f));
}

void CanvasGroup::ProcessCanvas(GuiGeometryBuilder* builder)
{
	assert_not_null(builder);
	mPositions = std::stack<Vector2>();
	mGeometryBuilder = builder;
	this->OnProcessCanvas(builder);
}

void CanvasGroup::OnProcessCanvas(GuiGeometryBuilder* builder)
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
	const uint32 titleHeight = 25;

	static const Color titleTop = Color::HexToRGB("#EEEEEE");
	static const Color titleBottom = Color::HexToRGB("#777777");

	const Vector2 absolutePosition = GetAbsolutePosition(position);

	// Add shadow
	if(mShadowOffset > 0) {
		mGeometryBuilder->AddQuad(absolutePosition - Vector2(mShadowOffset, mShadowOffset),
			size + Size(mShadowOffset * 2, mShadowOffset * 2), mShadowColor);
	}

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
	const Vector2 absolutePosition = GetAbsolutePosition(position);

	// Add shadow
	if(mShadowOffset > 0) {
		mGeometryBuilder->AddQuad(absolutePosition - Vector2(mShadowOffset, mShadowOffset),
			size + Size(mShadowOffset * 2, mShadowOffset * 2), mShadowColor);
	}

	// Add body
	mGeometryBuilder->AddGradientQuad(absolutePosition, size, mBackColorTop, mBackColorBottom);

	if(text.length() > 0 && !mFont.IsNull()) {
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

bool CanvasGroup::Toggle(const Size& size, const Vector2& position, bool toggled, const playstate::string& text)
{
	const Vector2 absolutePosition = GetAbsolutePosition(position);

	const uint32 checkboxSize = 20U;
	const uint32 checkboxBorderOffset = 1U;
	const uint32 checkboxToggleOffset = checkboxBorderOffset + 3U;
	
	// Add border around checkbox
	mGeometryBuilder->AddQuad(absolutePosition, Size(checkboxSize, checkboxSize), Color::Black);

	// Add Checkbox bg
	mGeometryBuilder->AddQuad(absolutePosition + Vector2(checkboxBorderOffset, checkboxBorderOffset), 
		Size(checkboxSize - (checkboxBorderOffset * 2), checkboxSize - (checkboxBorderOffset * 2)), mBackColorTop);

	if(toggled) {
		mGeometryBuilder->AddQuad(absolutePosition + Vector2(checkboxToggleOffset, checkboxToggleOffset), 
			Size(checkboxSize - (checkboxToggleOffset * 2), checkboxSize - (checkboxToggleOffset * 2)), Color::White);
	}

	// Add body
	//mGeometryBuilder->AddGradientQuad(absolutePosition + Vector2(checkboxSize + 5.0f, 0.0f), size, mBackColorTop, mBackColorBottom);
	
	// Add text next to the body of the checkbox
	if(text.length() > 0 && !mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), absolutePosition + Vector2(checkboxSize + 10.0f, 0.0f), mFrontColor, text, size);
	}

	if(mCanvas->GetMouseClick() == MouseButtons::LEFT) {
		// Check if mouse is over current position
		const Point mousePos = mCanvas->GetMousePosition();
		if(Rect(absolutePosition.X, absolutePosition.Y, size.Width, size.Height).IsPointInside(mousePos)) {
			toggled = !toggled;
		}
	}

	return toggled;
}

float32 CanvasGroup::Slider(const Size& size, const Vector2& position, float32 value, float32 leftValue, float32 rightValue,
			float32 stepValue)
{
	return 0.0f;
}

uint32 CanvasGroup::ComboBox(const Size& size, const Vector2& position, uint32 selectedIndex)
{
	return 0;
}
