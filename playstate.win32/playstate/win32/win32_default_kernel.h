#pragma once
#include <playstate/kernel.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

#include <playstate/logging/console/console_logger.h>
#include <playstate/processor/processors/linked_list_update_processor.h>
#include <playstate/processor/processors/octree_light_source_processor.h>
#include <playstate/processor/processors/octree_render_processor.h>

// TODO: Interface these services
#include <playstate/resources/threaded/threaded_resource_manager.h>
#include <playstate/script/script_system.h>
#include <playstate/rendering/render_system.h>

#include "window/win32_window.h"
#include "input/win32_input_system.h"
#include "filesystem/win32_filesystem.h"
#include "rendering/win32_graphics_driver.h"
#include "thread/win32_thread_factory.h"

namespace playstate
{
	namespace win32
	{
		//
		// The default kernel for the windows version of this game engine. Use this unless you are absolutely sure what you are doing
		class Win32DefaultKernel : public IKernel
		{
		public:
			Win32DefaultKernel(HINSTANCE app, const std::string& fileSystemPath);
			virtual ~Win32DefaultKernel();

		// IKernel
		public:
			virtual void Initialize();
			virtual void Release();
			virtual void Process();
			virtual void RegisterScript();

		private:
			HINSTANCE mAppInstance;

			Win32Window* mWindow;
			ConsoleLogger* mLogger;
			LinkedListUpdateProcessorFactory* mUpdateProcessorFactory;
			OctreeRenderProcessorFactory* mRenderProcessorFactory;
			OctreeLightSourceProcessorFactory* mLightSourceProcessorFactory;
			Win32InputSystem* mInputSystem;
			Win32GraphicsDriver* mGraphicsDriver;
			RenderSystem* mRenderSystem;
			Win32ThreadFactory* mThreadFactory;
			ThreadedResourceManager* mThreadedResourceManager;
			Win32FileSystem* mFileSystem;
			ScriptSystem* mScriptSystem;
		};
	}
}
