#include "../memory/memory.h"
#include "gui_widget.h"
#include "canvas_group.h"
using namespace playstate;

GuiWidget::GuiWidget()
	: mCanvasGroup(NULL), mParent(NULL), 
	mEvents(offsetof(GuiEvent, EventLink)), mChildren(offsetof(GuiWidget, GuiWidgetLink))
{
	mBackColorTop = Color::HexToRGB("#99");
	mBackColorBottom = Color::HexToRGB("#22");
}

GuiWidget::~GuiWidget()
{
	mEvents.DeleteAll();
	mChildren.DeleteAll();	
	mCanvasGroup = NULL;
	mParent = NULL;
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
	if(IsAttachedToGroup())
		DetachingWidgetFromCanvasGroup(group);

	mCanvasGroup = group;
	UpdateStyle(group->GetStyle());

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

void GuiWidget::SetPosition(const Vector2& position)
{
	Vector2 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;

	GuiWidget* widget = mChildren.First();
	while(widget != NULL) {
		widget->UpdatePosition();
		widget = widget->GuiWidgetLink.Tail;
	}
}

void GuiWidget::UpdateStyle(const GuiStyle& style)
{
	mBackColorTop = style.FindColor(SAFE_STRING("BackColor.Top"), Color::HexToRGB("#99"));
	mBackColorBottom = style.FindColor(SAFE_STRING("BackColor.Bottom"), Color::HexToRGB("#22"));
	this->OnStyleChanged(style);
}

void GuiWidget::SetSize(const Vector2& size)
{
	mSize = size;
}

void GuiWidget::OnStyleChanged(const GuiStyle& style)
{
}

const void GuiWidget::BuildWidgetGeometry(GuiGeometryBuilder& builder)
{
	GuiWidget* child = mChildren.First();
	while(child != NULL) {
		child->BuildWidgetGeometry(builder);
		child = child->GuiWidgetLink.Tail;
	}
}

void GuiWidget::AddWidget(GuiWidget* widget)
{
	assert_not_null(widget);

	mChildren.AddLast(widget);
	widget->mParent = this;

	widget->UpdatePosition();
	widget->UpdateStyle(mCanvasGroup->GetStyle());
}

void GuiWidget::RemoveWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	assert(widget->mParent == this && "You are not allowed to remove another scene objects child node");
	mChildren.Remove(widget);
	widget->mParent = NULL;
}

void GuiWidget::UpdatePosition()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsolutePosition = mParent->mAbsolutePosition + mPosition;

	GuiWidget* widget = mChildren.First();
	while(widget != NULL) {
		widget->UpdatePosition();
		widget = widget->GuiWidgetLink.Tail;
	}
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

int playstate::GuiWidget_SetSize(lua_State* L)
{
	Vector2 size = luaM_popvector2(L);
	GuiWidget* widget = luaM_popobject<GuiWidget>(L);
	if(widget) {
		widget->SetSize(size);
	}

	return 0;
}

int playstate::GuiWidget_AddWidget(lua_State* L)
{
	GuiWidget* child = luaM_popobject<GuiWidget>(L);
	GuiWidget* self = luaM_popobject<GuiWidget>(L);
	if(self != NULL && child != NULL) {
		self->AddWidget(child);
	}
	return 0;
}

int playstate::GuiWidget_RemoveWidget(lua_State* L)
{
	GuiWidget* child = luaM_popobject<GuiWidget>(L);
	GuiWidget* self = luaM_popobject<GuiWidget>(L);
	if(self != NULL && child != NULL) {
		self->RemoveWidget(child);
	}
	return 0;
}
