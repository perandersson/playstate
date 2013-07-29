#pragma once
#include "singleton.h"

namespace playstate
{
	class IWindow;
	class ILogger;
	class IUpdateProcessorFactory;
	class IRenderProcessorFactory;
	class ILightSourceProcessorFactory;
	class IInputSystem;
	class IGraphicsDriver;
	class IRenderSystem;
	class IThreadFactory;
	class IResourceManager;
	class IFileSystem;

	//
	// {@code
	//	IKernel& kernel = IKernel::Get();
	//	IWindow& window = kernel.Window;
	// }
	class IKernel : public Singleton<IKernel>
	{
	protected:
		IKernel();

	public:
		//
		// Initializes the game engine kernel. Used to create the neccessary service dependencies, such as the script engine.
		// @throw playstate::Exception If the initialization of the kernel failed.
		virtual void Initialize() = 0;

		//
		// Releases this kernels internal resources.
		virtual void Release() = 0;

	public:
		IWindow*& const Window;
		ILogger*& const Logger;
		IUpdateProcessorFactory*& const UpdateProcessorFactory;
		IRenderProcessorFactory*& const RenderProcessorFactory;
		ILightSourceProcessorFactory*& const LightSourceProcessorFactory;
		IInputSystem*& const InputSystem;
		IGraphicsDriver*& const GraphicsDriver;
		IRenderSystem*& const RenderSystem;
		IThreadFactory*& const ThreadFactory;
		IResourceManager*& const ResourceManager;
		IFileSystem*& const FileSystem;

	protected:
		IWindow* mWindow;
		ILogger* mLogger;
		IUpdateProcessorFactory* mUpdateProcessorFactory;
		IRenderProcessorFactory* mRenderProcessorFactory;
		ILightSourceProcessorFactory* mLightSourceProcessorFactory;
		IInputSystem* mInputSystem;
		IGraphicsDriver* mGraphicsDriver;
		IRenderSystem* mRenderSystem;
		IThreadFactory* mThreadFactory;
		IResourceManager* mResourceManager;
		IFileSystem* mFileSystem;
	};
}
