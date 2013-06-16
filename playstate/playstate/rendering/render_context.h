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

		//
		// If this render context consists of multiple buffers then this method swaps those buffers.
		virtual void SwapBuffers() = 0;
	};

}

