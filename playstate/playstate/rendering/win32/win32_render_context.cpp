#include "../../memory/memory.h"
#include "win32_render_context.h"
using namespace playstate;

Win32RenderContext::Win32RenderContext(HDC deviceContext, HGLRC renderContext) 
	: mDeviceContext(deviceContext), mRenderContext(renderContext)
{
}

Win32RenderContext::~Win32RenderContext()
{
	if(mRenderContext != NULL) {
		wglDeleteContext(mRenderContext);
		mRenderContext = NULL;
	}
}

void Win32RenderContext::MakeCurrent()
{
	wglMakeCurrent(mDeviceContext, mRenderContext);
}

void Win32RenderContext::Unbind()
{
	wglMakeCurrent(NULL, NULL); 
}
