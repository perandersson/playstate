#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#include <float.h>
// Unterupt for when floating points become invalid
// NOT USABLE WHEN USING gDEBugger
unsigned int fp_control_state = _controlfp(_EM_INEXACT | _EM_INVALID | _EM_UNDERFLOW | _EM_OVERFLOW, _MCW_EM);
#endif

#include <playstate/playstate.h>

#include "../pipeline/deferred_render_pipeline.h"
#include "../demo.h"
#include <vector>
#include <set>
using namespace playstate;

#ifdef _DEBUG
int main(int argc, char** argv)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
#endif
{
#ifdef _DEBUG
	// Enable windows memory leak detection when running in debug mode
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
#endif

	try
	{
		// Filesystem
		std::vector<std::string> paths;
		paths.push_back(std::string("data"));
		Win32FileSystem fileSystem(paths);
		
		// Script integratrion
		ScriptSystem ss(fileSystem);
		ss.RegisterType("Window", IWindow_Methods);
		ss.RegisterType("IWindowClosedListener", IWindowClosedListener_Methods);
		ss.RegisterType("IGame", IGame_Methods);
		ss.RegisterType("Game", Game_Methods);
		ss.RegisterType("Keys", IInputSystem_Keys_Methods);
		ss.RegisterType("Mouse", IInputSystem_Mouse_Methods);
		ss.RegisterType("Scene", Scene_Methods);
		ss.RegisterType("SceneGroup", SceneGroup_Methods);
		ss.RegisterType("SceneNode", SceneNode_Methods);
		ss.RegisterType("PointLight", SceneNode_Methods);
		ss.RegisterType("PointLight", PointLight_Methods);
		ss.RegisterType("Component", Component_Methods);
		ss.RegisterType("Model", Model_Methods);
		ss.RegisterType("RenderStaticModel", RenderStaticModel_Methods);
		ss.RegisterType("ActiveCamera", ActiveCamera_Methods);
		ss.RegisterType("DeferredRenderPipeline", DeferredRenderPipeline_Methods);

		// Linked list update processor
		LinkedListUpdateProcessorFactory linkedListUpdateProcessorFactory;

		// Render processor
		OctreeRenderProcessorFactory octreeRenderProcessorFactory;
	
		// Window 
		Win32Window window(GetModuleHandle(NULL), 320, 240, "");

		// Input
		Win32InputSystem inputSystem(window);

		// Create and register a graphics driver
		Win32GraphicsDriver gfxDriver(window);
		
		// Thread management
		Win32ThreadFactory threadFactory;
		
		// Create and register a render system
		RenderSystem renderSystem(window, ss);

		// Resource management
		ResourceManager resourceManager(renderSystem, fileSystem);

		// Initialize the scripting engine and start the application
		ss.CompileFile("/main.lua")->Execute("main()");
	} 
	catch(Exception e)
	{
#undef GetMessage
		MessageBox(NULL, e.GetMessage().c_str(), "Unhandled exception", MB_OK | MB_ICONERROR);
	}
	
	return 0;
}
