#pragma once

namespace playstate
{
	class IThread;

	class IRunnable
	{
	public:
		IRunnable() {}
		virtual ~IRunnable() {}

		//
		// Method called when the runnable thread is started
		// @param thread The thread where this runnable instance is running from.
		virtual void Run(IThread& thread) = 0;
	};
}
