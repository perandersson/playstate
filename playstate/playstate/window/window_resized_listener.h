#pragma once
#include "../types.h"

namespace playstate
{
	class IWindow;

	class IWindowResizedListener
	{
	public:
		virtual ~IWindowResizedListener() {}

	public:
		//
		// Method called when the supplied window has been resized.
		// @param window
		// @param width
		// @param height
		virtual void OnWindowResized(IWindow& window, uint32 width, uint32 height) = 0;
	};
}
