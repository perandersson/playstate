#pragma once
#include "../types.h"
#include "../script/scriptable.h"
#include "../linked_list.h"

namespace playstate
{
	class GuiWidget;

	class GuiEvent
	{
	public:
		LinkedListLink<GuiEvent> EventLink;

	public:
		GuiEvent();
		GuiEvent(type_mask typeMask);
		virtual ~GuiEvent();

	public:
		void OnAttachedToCanvas(GuiWidget* widget);
		void OnDetachingFromCanvas(GuiWidget* widget);

		//
		// @return The widget this event is connected to.
		GuiWidget* GetWidget() const;

		type_mask GetTypeMask() const;

		void SetTypeMask(type_mask typeMask);

	protected:
		//
		// Method called when this event is added to the canvas through a GuiWidget.
		//
		// @remark If you add this instance type to a GuiWidget; it will not invoke this method
		//	unless it's added (or being added) to a CanvasGroup. 
		virtual void OnEventAdded();

		//
		// Method called when this event is removed from the canvas
		virtual void OnEventRemoved();

	private:
		GuiWidget* mWidget;
		type_mask mTypeMask;
	};
}
