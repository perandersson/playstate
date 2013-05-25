#include "../../memory/memory.h"
#include "win32_render_system.h"
#include "win32_render_context.h"
#include <gl/glew.h>
#include <gl/wglew.h>
using namespace playstate;

Win32RenderSystem::Win32RenderSystem(Win32Window& window, ScriptSystem& scriptSystem)
	: RenderSystem(window, scriptSystem), mWindowHandle(window.GetWindowHandle()), mRenderContext(NULL), mDeviceContext(NULL)
{
}

Win32RenderSystem::~Win32RenderSystem()
{
	if(mRenderContext != NULL) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mRenderContext);
		mRenderContext = NULL;
	}

	if(mDeviceContext != NULL) {
		ReleaseDC(mWindowHandle, mDeviceContext);
		mDeviceContext = NULL;
	}
}

void Win32RenderSystem::Initialize()
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

	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	mRenderContext = wglCreateContextAttribsARB(mDeviceContext, 0, attribs);
	if(!mRenderContext) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}

	// Unbind and delete the temporary context.
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);
	
	wglMakeCurrent(mDeviceContext, mRenderContext);
	RenderSystem::Initialize();
}

void Win32RenderSystem::SwapBuffers()
{
	::SwapBuffers(mDeviceContext);
}

IRenderContext* Win32RenderSystem::CreateRenderContext()
{
	int attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};

	HGLRC context = wglCreateContextAttribsARB(mDeviceContext, mRenderContext, attribs);
	if(!context) {
		THROW_EXCEPTION(RenderingException, "You'r graphics card does not support OpenGL 3.3");
	}
	return new Win32RenderContext(mDeviceContext, context);
}
