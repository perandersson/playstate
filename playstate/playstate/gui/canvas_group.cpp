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

	mSliderControl.SetStyle(style);
	mLabelControl.SetStyle(style);
	mCheckboxControl.SetStyle(style);
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
	if(mFont.IsNotNull() && !title.empty()) {		
		static const uint32 titleHeight = 25;
		static const Color titleTop = Color::HexToRGB("#EEEEEE");
		static const Color titleBottom = Color::HexToRGB("#777777");

		const Rect titleCoords(coordinates.Position, Size(coordinates.Width, titleHeight));
		mGeometryBuilder->AddGradientQuad(titleCoords, titleTop, titleBottom);
		mGeometryBuilder->AddText(mFont.Get(), coordinates.Position, mFrontColor, title);
		titleOffset = titleHeight;
	}

	// Add body
	const Rect bodyCoords(coordinates.Position + Point(0, titleOffset), coordinates.Size - Size(0, titleOffset));
	mGeometryBuilder->AddGradientQuad(bodyCoords, mBackColorTop, mBackColorBottom);

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
