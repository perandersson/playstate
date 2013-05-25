#pragma once

namespace playstate
{
	class IWindow;

	class IWindowClosedListener
	{
	public:
		virtual ~IWindowClosedListener() {}

	public:
		//
		// Method called when the supplied window is closing
		// @return TRUE if we are allowing the window to be closed; FALSE otherwise
		virtual bool OnWindowClosing(IWindow& window) = 0;
	};
}
