#include "../memory/memory.h"
#include "gui_widget.h"
#include "canvas_group.h"
using namespace playstate;

GuiWidget::GuiWidget()
	: mCanvasGroup(NULL), mParent(NULL), mEvents(offsetof(GuiEvent, EventLink))
{
}

GuiWidget::GuiWidget(CanvasGroup* group)
	: mCanvasGroup(NULL), mParent(NULL), mEvents(offsetof(GuiEvent, EventLink))
{
	assert_not_null(group);
	group->AddWidget(this);
}

GuiWidget::GuiWidget(GuiWidget* parent)
	: mCanvasGroup(NULL), mParent(parent), mEvents(offsetof(GuiEvent, EventLink))
{
	assert_not_null(parent);
	if(parent != NULL)
		mCanvasGroup = parent->GetCanvasGroup();

	if(mCanvasGroup != NULL)
		mCanvasGroup->AddWidget(this);
}

GuiWidget::~GuiWidget()
{
	mEvents.DeleteAll();

	Children::iterator it = mChildren.begin();
	while(it != mChildren.end()) {
		GuiWidget* widget = *it;
		it++;
		widget->mParent = NULL;
		delete widget;
	}

	if(mParent != NULL) {
		mParent->RemoveWidget(this);
	}
	
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
	Vector2 diff = position - mPosition;
	mPosition = position;
	mAbsolutePosition += diff;
	
	Children::size_type size = mChildren.size();
	for(Children::size_type i = 0; i < size; ++i) {
		mChildren[i]->UpdatePosition();
	}
}

const Vector2& GuiWidget::GetAbsolutePosition() const
{
	return mAbsolutePosition;
}

void GuiWidget::SetSize(const Vector2& size)
{
	mSize = size;
}

const void GuiWidget::BuildWidgetGeometry(GuiGeometryBuilder& builder) const
{

}

void GuiWidget::AddWidget(GuiWidget* widget)
{
	assert_not_null(widget);

	mChildren.push_back(widget);
	widget->mParent = this;

	// Remove this nodes link from the scene to prevent the LinkedList implementation from removing the "List->Next" element
	//widget->RemoveFromScene();

	widget->UpdatePosition();
	//widget->UpdateRotation();
}

void GuiWidget::RemoveWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	assert(widget->mParent == this && "You are not allowed to remove another scene objects child node");

	Children::iterator it = std::find(mChildren.begin(), mChildren.end(), widget);
	if(it != mChildren.end()) {
		mChildren.erase(it);
		widget->mParent = NULL;
	}
}

void GuiWidget::UpdatePosition()
{
	assert(mParent != NULL && "Illegal call when no parent is found for this object");
	mAbsolutePosition = mParent->mAbsolutePosition + mPosition;

	Children::size_type size = mChildren.size();
	for(Children::size_type i = 0; i < size; ++i) {
		mChildren[i]->UpdatePosition();
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
