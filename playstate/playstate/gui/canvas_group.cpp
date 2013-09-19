#include "../memory/memory.h"
#include "canvas_group.h"
#include "../processor/processors/linked_list_update_processor.h"
#include "../script/script_system.h"
#include "../math/math.h"
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
	mPositions = std::stack<Point>();
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

void CanvasGroup::SetName(const playstate::string& name)
{
	mName = name;
}

Rect CanvasGroup::GetAbsoluteCoordinates(const Rect& rect) const
{
	if(mPositions.empty())
		return rect;

	return rect.GetTranslated(mPositions.top());
}

void CanvasGroup::BeginFrame(const Rect& rect, const playstate::string& title)
{
	const uint32 titleHeight = 25;

	static const Color titleTop = Color::HexToRGB("#EEEEEE");
	static const Color titleBottom = Color::HexToRGB("#777777");

	const Rect coordinates = GetAbsoluteCoordinates(rect);

	// Add shadow
	if(mShadowOffset > 0) {
		const Rect shadowCoords(coordinates.Position - Point(mShadowOffset, mShadowOffset),
			coordinates.Size + Size(mShadowOffset * 2, mShadowOffset * 2));
		mGeometryBuilder->AddQuad(shadowCoords, mShadowColor);
	}

	// Add title
	const Rect titleCoords(coordinates.Position, Size(coordinates.Width, titleHeight));
	mGeometryBuilder->AddGradientQuad(titleCoords, titleTop, titleBottom);

	// Add body
	const Rect bodyCoords(coordinates.Position + Point(0, titleHeight), coordinates.Size - Size(0, titleHeight));
	mGeometryBuilder->AddGradientQuad(bodyCoords, mBackColorTop, mBackColorBottom);

	if(!mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), coordinates.Position, mFrontColor, title);
	}

	mPositions.push(coordinates.Position);
}

void CanvasGroup::EndFrame()
{
	if(mPositions.size() > 0)
		mPositions.pop();
}

bool CanvasGroup::Button(const Rect& rect, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);

	// Add shadow
	if(mShadowOffset > 0) {
		const Rect shadowCoords(coordinates.Position - Point(mShadowOffset, mShadowOffset),
			coordinates.Size + Size(mShadowOffset * 2, mShadowOffset * 2));
		mGeometryBuilder->AddQuad(shadowCoords, mShadowColor);
	}

	// Add body
	mGeometryBuilder->AddGradientQuad(coordinates, mBackColorTop, mBackColorBottom);

	if(text.length() > 0 && !mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), coordinates.Position, mFrontColor, text);
	}

	if(mCanvas->GetMouseClick() == MouseButtons::LEFT) {
		const Point mousePos = mCanvas->GetMousePosition();
		if(coordinates.IsPointInside(mousePos)) {
			return true;
		}
	}

	return false;
}

void CanvasGroup::Label(const Rect& rect, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	mGeometryBuilder->AddText(mFont.Get(), coordinates.Position, mFrontColor, text);
}

bool CanvasGroup::Checkbox(const Rect& rect, bool toggled, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);

	const uint32 checkboxSize = 20U;
	const uint32 checkboxBorderOffset = 1U;
	const uint32 checkboxToggleOffset = checkboxBorderOffset + 3U;
	
	// Add border around checkbox button
	const Rect checkboxButtonBorder(coordinates.Position, Size(checkboxSize, checkboxSize));
	mGeometryBuilder->AddQuad(checkboxButtonBorder, Color::Black);

	// Add checkbox button
	const Rect checkboxButton(coordinates.Position + Point(checkboxBorderOffset, checkboxBorderOffset), 
		Size(checkboxSize - (checkboxBorderOffset * 2), checkboxSize - (checkboxBorderOffset * 2)));
	mGeometryBuilder->AddQuad(checkboxButton, mBackColorTop);

	// Add geometry if checkbox is toggled (enabled)
	if(toggled) {
		const Rect toggledCoords(coordinates.Position + Point(checkboxToggleOffset, checkboxToggleOffset), 
			Size(checkboxSize - (checkboxToggleOffset * 2), checkboxSize - (checkboxToggleOffset * 2)));
		mGeometryBuilder->AddQuad(toggledCoords, Color::White);
	}

	// Add checkbox text if one is present.
	if(text.length() > 0 && !mFont.IsNull()) {
		mGeometryBuilder->AddText(mFont.Get(), coordinates.Position + Point(checkboxSize + 10, 0), mFrontColor, text);
	}

	if(mCanvas->GetMouseClick() == MouseButtons::LEFT) {
		const Point mousePos = mCanvas->GetMousePosition();
		if(coordinates.IsPointInside(mousePos)) {
			toggled = !toggled;
		}
	}

	return toggled;
}

float32 CanvasGroup::Slider(const Rect& rect, float32 value, float32 leftValue, float32 rightValue,
			float32 stepValue)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);

	// Slider contains

	// Add body
	mGeometryBuilder->AddGradientQuad(coordinates, mBackColorTop, mBackColorBottom);

	// Add slider value
	const float valueDt = (leftValue + (rightValue - leftValue));

	const float32 valueOffset = rightValue - (rightValue - leftValue);
	const float32 dt = (value - valueOffset) / (rightValue - valueOffset);
	const Rect filledValueCoords(coordinates.Position, Size(coordinates.Width * dt, coordinates.Height));
	mGeometryBuilder->AddQuad(filledValueCoords, Color::White);
	
	if(mCanvas->GetMouseDown() == MouseButtons::LEFT) {
		const Point mousePos = mCanvas->GetMousePosition();
		if(coordinates.IsPointInside(mousePos)) {
			// Calculate value based on the mouse coordinates in relation to this control
			const float32 pixelsFromControlWidth = mousePos.X - coordinates.X;
			const float32 mouseDt = pixelsFromControlWidth / (float32)coordinates.Width;

			// Lerp between leftValue and rightValue
			value = Math::Lerp(leftValue, rightValue, mouseDt);
		}
	}

	char tmp[50];
	sprintf(tmp, "%.2f", value);

	mGeometryBuilder->AddText(mFont.Get(), coordinates.Position + Point(coordinates.Width / 2.0f, 0), mFrontColor, tmp, FontAlign::CENTER);

	return value;
}

uint32 CanvasGroup::ComboBox(const Rect& rect, uint32 selectedIndex)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	return 0;
}
