#pragma once
#include "../../input/input_state.h"

namespace playstate
{
	class IMouseEvent
	{
	public:
		virtual ~IMouseEvent() {}

	public:
		virtual void OnMouseClicked(MouseKeys key) = 0;
		virtual void OnMouseEnter() = 0;
		virtual void OnMouseExit() = 0;
	};

	class MouseEvent : public IMouseEvent
	{
	};
}
