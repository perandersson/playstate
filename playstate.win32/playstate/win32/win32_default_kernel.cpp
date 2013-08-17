#include <playstate/memory/memory.h>
#include "win32_default_kernel.h"
#include <playstate/game/game_runner.h>
#include <playstate/component/scriptable_component.h>
#include <playstate/model/model.h>
#include <playstate/component/renderable/render_static_model.h>
#include <playstate/component/lightsources/point_light.h>
#include <playstate/gui/widgets/gui_frame_widget.h>
#include <playstate/gui/widgets/gui_button_widget.h>

using namespace playstate;
using namespace playstate::win32;

Win32DefaultKernel::Win32DefaultKernel(HINSTANCE app, const std::string& fileSystemPath) 
	: mAppInstance(app)
{
	mFileSystem = new Win32FileSystem(fileSystemPath);
	mLogger = new ConsoleLogger();
	mWindow = new Win32Window(app);
	mInputSystem = new Win32InputSystem(*mWindow);

	mScriptSystem = new ScriptSystem(*mFileSystem, *mLogger);
	mUpdateProcessorFactory = new LinkedListUpdateProcessorFactory();
	mRenderProcessorFactory = new OctreeRenderProcessorFactory();
	mLightSourceProcessorFactory = new OctreeLightSourceProcessorFactory();

	mGraphicsDriver = new Win32GraphicsDriver(*mWindow);
	mThreadFactory = new Win32ThreadFactory();
	mRenderSystem = new RenderSystem(*mWindow, *mScriptSystem);
	mThreadedResourceManager = new ThreadedResourceManager(*mRenderSystem, *mFileSystem);
}

Win32DefaultKernel::~Win32DefaultKernel()
{
	delete mThreadedResourceManager;
	delete mRenderSystem;
	delete mThreadFactory;
	delete mGraphicsDriver;

	delete mLightSourceProcessorFactory;
	delete mRenderProcessorFactory;
	delete mUpdateProcessorFactory;

	delete mScriptSystem;

	delete mInputSystem;
	delete mWindow;
	delete mLogger;
	delete mFileSystem;
}

void Win32DefaultKernel::Initialize()
{
	RegisterScript();
}

void Win32DefaultKernel::Release()
{
}

void Win32DefaultKernel::Process()
{
	mWindow->HandleEvents();
	mFileSystem->Poll();
	mInputSystem->Poll();
	mThreadedResourceManager->Poll();
	mScriptSystem->HandleGC();
}

void Win32DefaultKernel::RegisterScript()
{
	// Move this into the kernel
	ScriptSystem& ss = ScriptSystem::Get();
	ss.RegisterType("Window", IWindow_Methods);
	ss.RegisterType("IWindowClosedListener", IWindowClosedListener_Methods);
	ss.RegisterType("IGame", IGame_Methods);
	ss.RegisterType("Game", Game_Methods);
	ss.RegisterType("Keys", IInputSystem_Keys_Methods);
	ss.RegisterType("Mouse", IInputSystem_Mouse_Methods);
	ss.RegisterType("Scene", Scene_Methods);
	ss.RegisterType("SceneGroup", SceneGroup_Methods);
	ss.RegisterType("SceneNode", SceneNode_Methods);
	ss.RegisterType("Component", Component_Methods);
	ss.RegisterType("Model", Model_Methods);
	ss.RegisterType("RenderStaticModel", RenderStaticModel_Methods);
	ss.RegisterType("ActiveCamera", ActiveCamera_Methods);
	ss.RegisterType("PointLight", PointLight_Methods);
	ss.RegisterType("Canvas", Canvas_Methods);
	ss.RegisterType("CanvasGroup", CanvasGroup_Methods);

	ss.RegisterType("GuiFrameWidget", GuiWidget_Methods);
	ss.RegisterType("GuiFrameWidget", GuiFrameWidget_Methods);

	ss.RegisterType("GuiButtonWidget", GuiWidget_Methods);
	ss.RegisterType("GuiButtonWidget", GuiButtonWidget_Methods);
}
