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
	mFont = style.FindResource<Font>(SAFE_STRING("Font"));
	mFont = style.FindResource<Font>(SAFE_STRING("Frame.Font"), mFont);

	mTitleColors[0] = style.FindColor(SAFE_STRING("Frame.Title.BackColor.Top"), Color::Black);
	mTitleColors[1] = style.FindColor(SAFE_STRING("Frame.Title.BackColor.Bottom"), Color::Black);
	mBodyColors[0] = style.FindColor(SAFE_STRING("Frame.Body.BackColor.Top"), Color::Black);
	mBodyColors[1] = style.FindColor(SAFE_STRING("Frame.Body.BackColor.Bottom"), Color::Black);

	mFontColor = style.FindColor(SAFE_STRING("Frame.FontColor"), Color::Black);

	mTitleHeight = (uint32)Math::Max(style.FindInt(SAFE_STRING("Frame.Title.Height"), 25), 0);
}

void FrameControl::Render(const Canvas& canvas, GuiGeometryBuilder* builder, const Rect& rect, const playstate::string& title)
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
}
