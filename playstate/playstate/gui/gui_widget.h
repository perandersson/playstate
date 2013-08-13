#pragma once
#include "../linked_list.h"
#include "../script/scriptable.h"
#include "gui_event.h"

namespace playstate
{
	class CanvasGroup;

	class GuiWidget : public Scriptable
	{
	public:
		LinkedListLink<GuiWidget> GuiWidgetLink;

	public:
		GuiWidget();
		GuiWidget(CanvasGroup* group);
		virtual ~GuiWidget();

		//
		// @return The canvas group this node is attached to
		CanvasGroup* GetCanvasGroup();

		//
		// Adds a new event listener for this widget.
		// 
		// @param event
		void AddEvent(GuiEvent* event);

		//
		// Removes the supplied event from this widget
		//
		// @param event
		void RemoveEvent(GuiEvent* event);

		//
		// Removes this widget from the canvas.
		void RemoveFromCanvas();
		
		//
		// Notifies this widget that it's been attached to the supplied group
		// 
		// This method might be called while it's already attached to a group. This
		//	means that this widget will be moved from one group to the next.
		//
		// @param group
		void WidgetAttachedToCanvasGroup(CanvasGroup* group);

		//
		// Notifies this widget that it's being detached from the supplied group
		void DetachingWidgetFromCanvasGroup(CanvasGroup* group);

		//
		// @return TRUE if this widget control is attached to a canvas group.
		bool IsAttachedToCanvasGroup() const;

	private:
		CanvasGroup* mCanvasGroup;
		LinkedList<GuiEvent, &GuiEvent::EventLink> mEvents;
	};
}