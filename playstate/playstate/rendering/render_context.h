#pragma once

namespace playstate
{
	class IRenderContext
	{
	public:
		virtual ~IRenderContext() {}

	public:
		//
		// Binds this render context to the current thread.
		virtual void MakeCurrent() = 0;

		//
		// Unbinds this render context from the current thread.
		virtual void Unbind() = 0;
	};

}

