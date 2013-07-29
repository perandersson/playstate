#include "memory/memory.h"
#include "kernel.h"
using namespace playstate;

template<> playstate::IKernel* playstate::Singleton<playstate::IKernel>::gSingleton = NULL;

IKernel::IKernel()
	: Singleton<IKernel>(), 
	mWindow(NULL), mLogger(NULL), mUpdateProcessorFactory(NULL), mRenderProcessorFactory(NULL), mLightSourceProcessorFactory(NULL),
	mInputSystem(NULL), mGraphicsDriver(NULL), mRenderSystem(NULL), mThreadFactory(NULL), mResourceManager(NULL), mFileSystem(NULL),
	Window(mWindow), Logger(mLogger), UpdateProcessorFactory(mUpdateProcessorFactory), RenderProcessorFactory(mRenderProcessorFactory), LightSourceProcessorFactory(mLightSourceProcessorFactory),
	InputSystem(mInputSystem), GraphicsDriver(mGraphicsDriver), RenderSystem(mRenderSystem), ThreadFactory(mThreadFactory), ResourceManager(mResourceManager), FileSystem(mFileSystem)
{
}
