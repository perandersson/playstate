#include <playstate/memory/memory.h>
#include "win32_render_context.h"
#include <GL/GL.h>
using namespace playstate;
using namespace playstate::win32;

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

void Win32RenderContext::SwapBuffers()
{
	::SwapBuffers(mDeviceContext);
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
	//if(error != GL_NO_ERROR)
		//THROW_EXCEPTION
}
