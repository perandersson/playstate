#include "../memory/memory.h"
#include "canvas_group.h"
using namespace playstate;

CanvasGroup::CanvasGroup()
	: mUpdateProcessor(IUpdateProcessorFactory::Get().Create())
{
	assert_not_null(mUpdateProcessor);
}

CanvasGroup::CanvasGroup(IUpdateProcessorFactory& updateProcessorFactory)
	: mUpdateProcessor(updateProcessorFactory.Create())
{
	assert_not_null(mUpdateProcessor);
}

CanvasGroup::~CanvasGroup()
{
	nWidgets.DeleteAll();
}

void CanvasGroup::AttachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->AttachUpdatable(updatable);
}

void CanvasGroup::DetachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->DetachUpdatable(updatable);
}

void CanvasGroup::AttachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->AttachTickable(tickable);
}

void CanvasGroup::DetachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->DetachTickable(tickable);
}

void CanvasGroup::Update()
{
	mUpdateProcessor->Update();
}

void CanvasGroup::AddWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	nWidgets.AddLast(widget);
}

void CanvasGroup::RemoveWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	nWidgets.Remove(widget);
}
