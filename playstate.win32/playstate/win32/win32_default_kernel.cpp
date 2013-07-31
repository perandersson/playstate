#include <playstate/memory/memory.h>
#include "win32_default_kernel.h"

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
	mResourceManager = new ResourceManager(*mRenderSystem, *mFileSystem);
}

Win32DefaultKernel::~Win32DefaultKernel()
{
	delete mResourceManager;
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
	//Window = new Win32Window();
}

void Win32DefaultKernel::Release()
{
	//delete Window;
}

void Win32DefaultKernel::Process()
{
	mWindow->HandleEvents();
	mInputSystem->Poll();
	mResourceManager->Poll();
	mScriptSystem->HandleGC();
}
