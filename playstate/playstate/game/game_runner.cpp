#include "../memory/memory.h"
#include "../kernel.h"
#include "game_runner.h"
#include "scripted_configuration.h"
#include "../model/wavefront/wavefront_resource_loader.h"
#include "../rendering/graphics_driver.h"
#include "../input/input_system.h"
#include "../font/truetype/truetype_font_resource_loader.h"
#include "../model/md5/md5_mesh_resource_loader.h"
#include "../timer/timer_factory.h"
#include "../image/image_resource_loader.h"
#include "../sound/sound_engine.h"
#include "../processor/processors/linked_list_update_processor.h"
using namespace playstate;

namespace playstate
{
	float32 GameDeltaTime = 0.0f;
	float64 GameTotalTime = 0.0f;
}

template<> playstate::GameRunner* playstate::Singleton<playstate::GameRunner>::gSingleton = NULL;

GameRunner::GameRunner(IGame* game, IConfiguration* configuration) 
	: mGame(game), mConfiguration(configuration), mRenderPipeline(NULL), mRunning(true),
	mCanvas(IWindow::Get(), IInputSystem::Get(), std::auto_ptr<IUpdateProcessor>(new LinkedListUpdateProcessor())), mTimer(NULL)
{
	assert_not_null(game);
	assert_not_null(configuration);
	mTimer = ITimerFactory::Get().CreateTimer();
}

GameRunner::~GameRunner()
{
	if(mTimer != NULL) {
		delete mTimer;
		mTimer = NULL;
	}

	if(mRenderPipeline != NULL) {
		delete mRenderPipeline;
		mRenderPipeline = NULL;
	}
}

void GameRunner::Start()
{
	if(!Initialize()) {
		Release();
		return;
	}

	if(!mGame->Initialize()) {
		mGame->Release();
		Release();
		return;
	}

	mGame->LoadContent();
	Run();
	mGame->UnloadContent();

	mGame->Release();
	Release();
}

void GameRunner::Shutdown()
{
	mRunning = false;
}

void GameRunner::Run()
{
	ScriptSystem& scriptSystem = ScriptSystem::Get();
	IKernel& kernel = IKernel::Get();
	IWindow& window = IWindow::Get();
	IRenderContext* screenRenderContext = IGraphicsDriver::Get().GetScreenRenderContext();

	while(mRunning) {
		GameDeltaTime = mTimer->GetElapsedAndRestart();
		GameTotalTime += (float64)GameDeltaTime;

		scriptSystem.SetGlobalVar("GameDeltaTime", GameDeltaTime);
		scriptSystem.SetGlobalVar("GameTotalTime", GameTotalTime);
		scriptSystem.SetGlobalVar("SecondsPerTick", SecondsPerTick);

		mScene.Update();
		mCanvas.Update();
		mGame->Update();

		if(mRenderPipeline != NULL)
			mRenderPipeline->Render(mScene, mCanvas, mScene.GetActiveCamera());

		screenRenderContext->SwapBuffers();
		kernel.Process();
	}
}

void GameRunner::SetRenderPipeline(IRenderPipeline* renderPipeline)
{
	if(mRenderPipeline != NULL)
		delete mRenderPipeline;
	
	mRenderPipeline = renderPipeline;
}

bool GameRunner::Initialize()
{
	IResourceManager& resourceManager = IResourceManager::Get();
	IFileSystem& fileSystem = IFileSystem::Get();
	IRenderSystem& renderSystem = IRenderSystem::Get();
	IWindow& window = IWindow::Get();
	ISoundEngine& soundEngine = ISoundEngine::Get();

	// Register resource types
	resourceManager.RegisterResourceType(new ImageResourceLoader(renderSystem, fileSystem), ".png");
	resourceManager.RegisterResourceType(new WavefrontResourceLoader(resourceManager, fileSystem, renderSystem), ".obj");
	resourceManager.RegisterResourceType(new TrueTypeFontResourceLoader(fileSystem, renderSystem), ".ttf");
	//resourceManager.RegisterResourceType(new MD5MeshResourceLoader(fileSystem, resourceManager, renderSystem), ".md5mesh");

	int32 width = mConfiguration->FindInt("window.width");
	int32 height = mConfiguration->FindInt("window.height");
	playstate::string title = mConfiguration->FindString("window.title");

	window.SetSize(Point(width, height));
	window.SetTitle(title);
	window.AddWindowClosedListener(this);

	// Set the sound volume
	soundEngine.SetMasterVolume(mConfiguration->FindFloat("sound.mastervolume", 0.5f));
	soundEngine.SetMusicVolume(mConfiguration->FindFloat("sound.musicvolume", 0.5f));
	soundEngine.SetSoundEffectVolume(mConfiguration->FindFloat("sound.soundeffectvolume", 0.5f));
	
	return true;
}

void GameRunner::Release()
{
	IWindow::Get().RemoveWindowClosedListener(this);
}

bool GameRunner::OnWindowClosing()
{
	mRunning = false;
	return true;
}

const Scene& GameRunner::GetScene() const
{
	return mScene;
}

Scene& GameRunner::GetScene()
{
	return mScene;
}

Canvas& GameRunner::GetCanvas()
{
	return mCanvas;
}

const Canvas& GameRunner::GetCanvas() const
{
	return mCanvas;
}
//
// Script integration
//

namespace playstate
{
	int Game_Start(lua_State* L)
	{
		ScriptedConfiguration* configuration = new ScriptedConfiguration(luaM_popcollection(L));
		ScriptableGame* game = luaM_getobject<ScriptableGame>(L);
		if(game != NULL) {
			GameRunner runner(game, configuration);
			runner.Start();

			delete game;
			delete configuration;
		}

		return 0;
	}

	int Game_SetRenderPipeline(lua_State* L)
	{
		int top1 = lua_gettop(L);
		IRenderPipeline* pipeline = luaM_popobject<IRenderPipeline>(L);
		int top2 = lua_gettop(L);
		if(pipeline != NULL) {
			GameRunner::Get().SetRenderPipeline(pipeline);
		}

		return 0;
	}

	int Game_Shutdown(lua_State* L)
	{
		GameRunner::Get().Shutdown();
		return 0;
	}
}