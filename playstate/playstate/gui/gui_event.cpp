#include "../memory/memory.h"
#include "gui_event.h"
using namespace playstate;

GuiEvent::GuiEvent()
	: mWidget(NULL), mTypeMask(BIT_ALL)
{
}

GuiEvent::GuiEvent(type_mask typeMask)
	: mWidget(NULL), mTypeMask(typeMask)
{
}

GuiEvent::~GuiEvent()
{
	this->OnEventRemoved();
}

void GuiEvent::OnAttachedToCanvas(GuiWidget* widget)
{
	mWidget = widget;
	this->OnEventAdded();
}

void GuiEvent::OnDetachingFromCanvas(GuiWidget* widget)
{
	this->OnEventRemoved();
	mWidget = NULL;
}

void GuiEvent::OnEventAdded()
{
}

void GuiEvent::OnEventRemoved()
{
}

GuiWidget* GuiEvent::GetWidget() const
{
	return mWidget;
}

type_mask GuiEvent::GetTypeMask() const
{
	return mTypeMask;
}

void GuiEvent::SetTypeMask(type_mask typeMask)
{
	mTypeMask = typeMask;
}

