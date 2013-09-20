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

	mShadowOffset = style.FindInt(SAFE_STRING("ShadowOffset"), 0);
	mShadowColor = style.FindColor(SAFE_STRING("ShadowColor"), Color(0, 0, 0, 0.3f));

	mSliderControl.SetStyle(style);
	mLabelControl.SetStyle(style);
	mCheckboxControl.SetStyle(style);
	mButtonControl.SetStyle(style);
	mFrameControl.SetStyle(style);
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
	const Rect coordinates = GetAbsoluteCoordinates(rect);

	// Add shadow (if necessary)
	AddShadowRect(coordinates);

	// Add border
	AddBorderRect(coordinates);

	// Add title
	uint32 titleOffset = 0;
	if(!title.empty()) {
		titleOffset = 25;
	}

	mFrameControl.Render(*mCanvas, mGeometryBuilder, coordinates, title);
	mPositions.push(coordinates.Position + Point(0, titleOffset));
}

Rect CanvasGroup::AddShadowRect(const Rect& rect)
{
	if(mShadowOffset == 0) {
		return Rect();
	}

	const Rect shadowCoords(rect.Position - Point(mShadowOffset, mShadowOffset),
		rect.Size + Size(mShadowOffset * 2, mShadowOffset * 2));
	mGeometryBuilder->AddQuad(shadowCoords, mShadowColor);
	return Rect(mShadowOffset, mShadowOffset, -mShadowOffset, -mShadowOffset);
}

Rect CanvasGroup::AddBorderRect(const Rect& rect)
{
	const int32 borderOffset = 1;
	const Color borderColor = Color::Black;
	const Rect shadowCoords(rect.Position - Point(borderOffset, borderOffset),
		rect.Size + Size(borderOffset * 2, borderOffset * 2));
	mGeometryBuilder->AddQuad(shadowCoords, borderColor);
	return Rect(borderOffset, borderOffset, -borderOffset, -borderOffset);
}

void CanvasGroup::EndFrame()
{
	if(mPositions.size() > 0)
		mPositions.pop();
}

bool CanvasGroup::Button(const Rect& rect, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	
	// Border
	AddBorderRect(coordinates);

	return mButtonControl.Render(*mCanvas, mGeometryBuilder, coordinates, text);;
}

void CanvasGroup::Label(const Rect& rect, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	mLabelControl.Render(*mCanvas, mGeometryBuilder, coordinates, text);
}

bool CanvasGroup::Checkbox(const Rect& rect, bool toggled, const playstate::string& text)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	return mCheckboxControl.Render(*mCanvas, mGeometryBuilder, coordinates, toggled, text);
}

float32 CanvasGroup::Slider(const Rect& rect, float32 value, float32 leftValue, float32 rightValue,
			float32 stepValue)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	
	// Border around the entire control
	AddBorderRect(coordinates);

	return mSliderControl.Render(*mCanvas, mGeometryBuilder, coordinates, value, leftValue, rightValue, stepValue);
}

uint32 CanvasGroup::ComboBox(const Rect& rect, uint32 selectedIndex)
{
	const Rect coordinates = GetAbsoluteCoordinates(rect);
	return 0;
}
