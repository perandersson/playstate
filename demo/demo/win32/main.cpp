#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#include <float.h>
// Unterupt for when floating points become invalid
// NOT USABLE WHEN USING gDEBugger
unsigned int fp_control_state = _controlfp(_EM_INEXACT | _EM_INVALID | _EM_UNDERFLOW | _EM_OVERFLOW, _MCW_EM);
#endif

#include <playstate/playstate.h>
#include <playstate/win32/win32_default_kernel.h>

#include "../pipeline/deferred_render_pipeline.h"
#include "../demo.h"
#include <vector>
#include <set>

using namespace playstate;
using namespace playstate::win32;

#ifdef WIN32
#undef GetMessage
#endif

class LuaSuffixFileListener : public IFileChangedListener
{
public:
	LuaSuffixFileListener(IFileSystem& fs)
	{
		// Listen for files that ends with .lua
		fs.AddFileChangedListener(std::regex("[^\\s]+\\.lua$"), this);
	}


// IFileChangedListener
public:
	virtual void FileChanged(const IFile& file, FileChangeAction::Enum action)
	{
		try {
			std::auto_ptr<Script> script = ScriptSystem::Get().CompileFile(file.GetPath());
			if(action == FileChangeAction::MODIFIED) {
				ILogger::Get().Debug("Updated script file: '%s'", file.GetPath().c_str());
				// Update all table functions associated with the files classes!
			} else if(action == FileChangeAction::ADDED) {
				ILogger::Get().Debug("Added script file: '%s'", file.GetPath().c_str());
			}
		} catch(ScriptException e) {
			ILogger::Get().Error("Could not update script file: '%s'. Reason: '%s'", file.GetPath().c_str(), e.GetMessage().c_str());
		}
	}
};

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

	Win32DefaultKernel kernel(GetModuleHandle(NULL), "data");
	kernel.Initialize();

	try
	{
		// Register game specific scripts.
		ScriptSystem& ss = ScriptSystem::Get();
		ss.RegisterType("DeferredRenderPipeline", DeferredRenderPipeline_Methods);

		// Attach a file change listener for .lua files
		LuaSuffixFileListener luaSuffixFileListener(IFileSystem::Get());

		// Initialize the scripting engine and start the application
		ss.CompileFile("/main.lua")->Execute("main()");

		kernel.Release();
	} 
	catch(Exception e)
	{
#undef GetMessage
		MessageBox(NULL, e.GetMessage().c_str(), "Unhandled exception", MB_OK | MB_ICONERROR);
	}
	
	return 0;
}
