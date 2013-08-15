#include "../memory/memory.h"
#include "gui_widget.h"
#include "canvas_group.h"
using namespace playstate;

GuiWidget::GuiWidget()
	: mCanvasGroup(NULL)
{
}

GuiWidget::GuiWidget(CanvasGroup* group)
	: mCanvasGroup(NULL)
{
	assert_not_null(group);
	group->AddWidget(this);
}

GuiWidget::~GuiWidget()
{
	mEvents.DeleteAll();
	mCanvasGroup = NULL;
}

CanvasGroup* GuiWidget::GetCanvasGroup()
{
	return mCanvasGroup;
}

void GuiWidget::AddEvent(GuiEvent* event)
{
	assert_not_null(event);
	mEvents.AddLast(event);
}

void GuiWidget::RemoveEvent(GuiEvent* event)
{
	assert_not_null(event);
	mEvents.Remove(event);
}

void GuiWidget::RemoveFromCanvas()
{
	if(mCanvasGroup != NULL)
		mCanvasGroup->RemoveWidget(this);
}

void GuiWidget::WidgetAttachedToCanvasGroup(CanvasGroup* group)
{
	if(IsAttachedToCanvasGroup())
		DetachingWidgetFromCanvasGroup(group);

	mCanvasGroup = group;

	GuiEvent* event = mEvents.First();
	while(event != NULL) {
		GuiEvent* event = event->EventLink.Tail;
		event->OnAttachedToCanvas(this);
		event = event;
	}
}

void GuiWidget::DetachingWidgetFromCanvasGroup(CanvasGroup* group)
{
	assert(mCanvasGroup == group && "Why are you trying to notify this node that it's being detached from someone elses group?");

	GuiEvent* event = mEvents.First();
	while(event != NULL) {
		GuiEvent* event = event->EventLink.Tail;
		event->OnDetachingFromCanvas(this);
		event = event;
	}

	mCanvasGroup = NULL;
}

bool GuiWidget::IsAttachedToCanvasGroup() const
{
	return mCanvasGroup != NULL;
}

const Vector2& GuiWidget::GetPosition() const
{
	return mPosition;
}

const Vector2& GuiWidget::GetSize() const
{
	return mSize;
}

void GuiWidget::SetPosition(const Vector2& position)
{
	mPosition = position;
}

const void GuiWidget::BuildWidgetGeometry(GuiGeometryBuilder& builder) const
{

}

int playstate::GuiWidget_SetPosition(lua_State* L)
{
	Vector2 position = luaM_popvector2(L);
	GuiWidget* widget = luaM_popobject<GuiWidget>(L);
	if(widget) {
		widget->SetPosition(position);
	}

	return 0;
}
