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
		//
		// @param newSize
		virtual void OnWindowResized(const Size& newSize) = 0;
	};
}
