#include <playstate/memory/memory.h>
#include "win32_default_kernel.h"
#include "filesystem/win32_filesystem.h"
#include "input/win32_input_system.h"
#include "window/win32_window.h"
#include <playstate/logging/console/console_logger.h>

using namespace playstate;
using namespace playstate::win32;

Win32DefaultKernel::Win32DefaultKernel(HINSTANCE app, const std::string& fileSystemPath) 
	: Kernel(), mAppInstance(app)
{
	mFileSystem = new Win32FileSystem(fileSystemPath);
	mLogger = new ConsoleLogger();
	Win32Window* window = new Win32Window(app);
	mWindow = window;
	mInputSystem = new Win32InputSystem(*window);
}

Win32DefaultKernel::~Win32DefaultKernel()
{
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
	// Poll resource manager
	// Update timers?
	// Update windows events
}
