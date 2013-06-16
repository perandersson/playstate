#include "../memory/memory.h"
#include "graphics_driver.h"
using namespace playstate;

IGraphicsDriver::IGraphicsDriver(IRenderContext*& screenRenderContext)
	: ScreenRenderContext(screenRenderContext),
	Singleton<IGraphicsDriver>()
{
}

IGraphicsDriver::~IGraphicsDriver()
{
}
