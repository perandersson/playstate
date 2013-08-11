#include "../memory/memory.h"
#include "canvas.h"
using namespace playstate;

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
	mGroups.DeleteAll();
}

void Canvas::AddCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	mGroups.AddLast(group);
}

void Canvas::RemoveCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	mGroups.Remove(group);
}

void Canvas::Update()
{
	CanvasGroup* group = mGroups.First();
	while(group != NULL) {
		CanvasGroup* next = group->CanvasGroupLink.Tail;
		group->Update();
		group = next;
	}
}
