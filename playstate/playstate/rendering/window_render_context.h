#pragma once
#include "../singleton.h"

namespace playstate
{
	class IWindowRenderContext : public Singleton<IWindowRenderContext>
	{
	public:
		virtual ~IWindowRenderContext() {}

	public:
		//
		// Swap the windows render context buffer
		virtual void SwapBuffers() = 0;
	};
}
