#include "../../memory/memory.h"
#include "frame_control.h"
#include "../canvas.h"
#include "../../math/math.h"
using namespace playstate;

FrameControl::FrameControl()
	: mTitleHeight(25U)
{
}

FrameControl::~FrameControl()
{
}

void FrameControl::SetStyle(const GuiStyle& style)
{
	static const playstate::string FONT_KEY(SAFE_STRING("Frame.Font>Font"));

	mFont = style.FindResource<Font>(FONT_KEY);
	
	static const playstate::string TITLE_BACK_TOP_COLOR(SAFE_STRING("Frame.Title.BackColor.Top>Frame.Title.BackColor>Frame.BackColor.Top>Frame.BackColor>BackColor.Top>BackColor"));
	static const playstate::string TITLE_BACK_BOTTOM_COLOR(SAFE_STRING("Frame.Title.BackColor.Bottom>Frame.Title.BackColor>Frame.BackColor.Bottom>Frame.BackColor>BackColor.Bottom>BackColor"));
	
	mTitleColors[0] = style.FindColor(TITLE_BACK_TOP_COLOR, Color::Black);
	mTitleColors[1] = style.FindColor(TITLE_BACK_BOTTOM_COLOR, Color::Black);
	
	static const playstate::string TITLE_FONT_COLOR(SAFE_STRING("Frame.Title.FontColor>FontColor"));

	mFontColor = style.FindColor(TITLE_FONT_COLOR, Color::Black);
	
	static const playstate::string TITLE_HEIGHT(SAFE_STRING("Frame.Title.Height"));

	mTitleHeight = (uint32)Math::Max(style.FindInt(TITLE_HEIGHT, 25), 0);

	static const playstate::string BODY_BACK_TOP_COLOR(SAFE_STRING("Frame.Body.BackColor.Top>Frame.Body.BackColor>Frame.BackColor.Top>Frame.BackColor>BackColor.Top>BackColor"));
	static const playstate::string BODY_BACK_BOTTOM_COLOR(SAFE_STRING("Frame.Body.BackColor.Bottom>Frame.Body.BackColor>Frame.BackColor.Bottom>Frame.BackColor>BackColor.Bottom>BackColor"));
	
	mBodyColors[0] = style.FindColor(BODY_BACK_TOP_COLOR, Color::Black);
	mBodyColors[1] = style.FindColor(BODY_BACK_BOTTOM_COLOR, Color::Black);
}

Point FrameControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& title)
{
	// Add title
	uint32 titleOffset = 0;
	if(mFont.IsNotNull() && !title.empty()) {
		const Rect titleCoords(rect.Position, Size(rect.Width, mTitleHeight));
		builder->AddGradientQuad(titleCoords, mTitleColors[0], mTitleColors[1]);
		builder->AddText(mFont.Get(), Rect(rect.X, rect.Y, rect.Width, mTitleHeight), mFontColor, title, FontAlign::CENTER);
		titleOffset = mTitleHeight;
	}

	// Add body
	const Rect bodyCoords(rect.Position + Point(0, titleOffset), rect.Size - Size(0, titleOffset));
	builder->AddGradientQuad(bodyCoords, mBodyColors[0], mBodyColors[1]);
	return rect.Position + Point(0, titleOffset);
}
