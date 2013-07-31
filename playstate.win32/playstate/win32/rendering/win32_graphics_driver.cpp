#include <playstate/memory/memory.h>
#include "win32_graphics_driver.h"
#include "win32_render_context.h"
#include <gl/glew.h>
#include <gl/wglew.h>
using namespace playstate;
using namespace playstate::win32;

template<> playstate::IGraphicsDriver* playstate::Singleton<playstate::IGraphicsDriver>::gSingleton = NULL;

Win32GraphicsDriver::Win32GraphicsDriver(Win32Window& window)
	: mWindowHandle(window.GetWindowHandle()), mScreenRenderContext(NULL), mDeviceContext(NULL)
{
	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	mDeviceContext = GetDC(mWindowHandle);
	if(!mDeviceContext) {
		THROW_EXCEPTION(RenderingException, "Could not create a device context for the current window");
	}

	int format = ChoosePixelFormat(mDeviceContext, &pfd);
	if(!format) {
		THROW_EXCEPTION(RenderingException, "Could not generate pixel format for the current window");
	}

	if(!SetPixelFormat(mDeviceContext, format, &pfd)) {
		THROW_EXCEPTION(RenderingException, "Could not set pixel format for the current window");
	}
	
	// Creates a temporary context. It's needed so that we can use
	// OpenGL related extensions. This will be removed as soon as 
	// a valid GL 3.1 context is created.
	HGLRC tempContext = wglCreateContext(mDeviceContext);
	if(!tempContext) {
		THROW_EXCEPTION(RenderingException, "Could not create an opengl rendering context for the current window");
	}

	wglMakeCurrent(mDeviceContext, tempContext);
	
	GLenum result = glewInit();
	if(result != GLEW_OK) {
		THROW_EXCEPTION(RenderingException, "Could not initialize glew");
	}

	if(wglewIsSupported("WGL_ARB_create_context") != 1) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.2");
	}

	mScreenRenderContext = CreateRenderContext();
	mScreenRenderContext->MakeCurrent();
	wglDeleteContext(tempContext);
}

Win32GraphicsDriver::~Win32GraphicsDriver()
{
	wglMakeCurrent(NULL, NULL);
	delete mScreenRenderContext;
	mScreenRenderContext = NULL;
	
	if(mDeviceContext != NULL) {
		ReleaseDC(mWindowHandle, mDeviceContext);
		mDeviceContext = NULL;
	}
}

IRenderContext* Win32GraphicsDriver::CreateRenderContext()
{
	return CreateRenderContext(mScreenRenderContext);
}

IRenderContext* Win32GraphicsDriver::CreateRenderContext(IRenderContext* context)
{
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC shareWithContext = context != NULL ? static_cast<Win32RenderContext*>(context)->RenderContext :  NULL;
	HGLRC windowsRenderContext = wglCreateContextAttribsARB(mDeviceContext, shareWithContext, attribs);
	if(!windowsRenderContext) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}
	return new Win32RenderContext(mDeviceContext, windowsRenderContext);
}

IRenderContext* Win32GraphicsDriver::GetScreenRenderContext()
{
	return mScreenRenderContext;
}
