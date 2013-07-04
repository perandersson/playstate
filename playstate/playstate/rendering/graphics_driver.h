#pragma once
#include "../singleton.h"
#include "render_context.h"
#include "exception/rendering_exception.h"

namespace playstate
{
	//
	// 
	class IGraphicsDriver : public Singleton<IGraphicsDriver>
	{
	protected:
		//
		// Property initializer constructor
		IGraphicsDriver(IRenderContext*& screenRenderContext);
		virtual ~IGraphicsDriver();

	public:
		//
		// Creates a new render context
		// @return
		virtual IRenderContext* CreateRenderContext() = 0;

		//
		// Creates and connects a new render context to another render context. 
		// The render contexts internal resources are shared between with the supplied one.
		// This is useful for when trying to perform graphics related functionality from multiple threads
		// while later be able to use them in the main thread.
		// @param context
		// @return A new render context instance
		virtual IRenderContext* CreateRenderContext(IRenderContext* context) = 0;

	public:
		// Read-only property for the render context used to draw onto the screen
		IRenderContext*& const ScreenRenderContext;
	};
}
