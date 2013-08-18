#pragma once
#include "../../input/input_state.h"

namespace playstate
{
	class IMouseEvent
	{
	public:
		virtual ~IMouseEvent() {}

	public:
		//
		// Event raised if the mouse clicked on the widget this event handle is attached to
		//
		// @param button The mouse button clicked.
		virtual void OnMouseClicked(MouseButtons::Enum button) = 0;
		virtual void OnMouseEnter() = 0;
		virtual void OnMouseExit() = 0;
	};
}
