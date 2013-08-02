#include "../memory/memory.h"
#include "gui_control.h"
#include "canvas_group.h"
using namespace playstate;

GuiControl::GuiControl(CanvasGroup* group)
	: mCanvasGroup(group)
{
	assert_not_null(group);
	mCanvasGroup->AddControl(this);
}

GuiControl::~GuiControl()
{
}

CanvasGroup* GuiControl::GetCanvasGroup()
{
	return mCanvasGroup;
}

const CanvasGroup* GuiControl::GetCanvasGroup() const
{
	return mCanvasGroup;
}
