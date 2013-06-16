#include "../memory/memory.h"
#include "game_runner.h"
#include "scripted_configuration.h"
#include "../model/wavefront/wavefront_resource_loader.h"
#include "../rendering/graphics_driver.h"
using namespace playstate;

namespace playstate
{
	float32 GameDeltaTime = 0.0f;
	float64 GameTotalTime = 0.0;
}

template<> playstate::GameRunner* playstate::Singleton<playstate::GameRunner>::gSingleton = NULL;

GameRunner::GameRunner()
	: mWindow(IWindow::Get()), mFileSystem(IFileSystem::Get()), mScriptSystem(ScriptSystem::Get()),
	mRenderSystem(RenderSystem::Get()), mResourceManager(ResourceManager::Get()),
	mGame(NULL), mConfiguration(NULL), mRenderPipeline(NULL), mRunning(true),
	mScreenRenderContext(IGraphicsDriver::Get().ScreenRenderContext),
	ActiveScene(mScene)
{
}

GameRunner::~GameRunner()
{
	if(mRenderPipeline != NULL) {
		delete mRenderPipeline;
		mRenderPipeline = NULL;
	}
}

void GameRunner::Start(IGame* game, IConfiguration* configuration)
{
	assert_not_null(game);
	assert_not_null(configuration);

	mGame = game;
	mConfiguration = configuration;

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

	while(mRunning) {
		GameDeltaTime = mWindow.GetTimeSinceLastUpdate();
		GameTotalTime += (float64)GameDeltaTime;

		mScriptSystem.SetGlobalVar("GameDeltaTime", GameDeltaTime);
		mScriptSystem.SetGlobalVar("GameTotalTime", GameTotalTime);

		mScene.Update();
		mGame->Update();
		mRenderPipeline->Render(&mScene, &mScene.ActiveCamera);
		mGame->Render();
		
		mScreenRenderContext->SwapBuffers();
		mWindow.HandleEvents();
		mResourceManager.Poll();
	}

	mGame->UnloadContent();

	mGame->Release();
	Release();
}

SceneGroup* GameRunner::LoadLevel(const std::string& fileName)
{
	std::auto_ptr<Script> script = ScriptSystem::Get().CompileFile(fileName);
	SceneGroup* grp = script->ReadInstance<SceneGroup>();
	return grp;
}

void GameRunner::SetRenderPipeline(IRenderPipeline* renderPipeline)
{
	if(mRenderPipeline != NULL) {
		delete mRenderPipeline;
	}
	mRenderPipeline = renderPipeline;
}

bool GameRunner::Initialize()
{
	// Register resource types
	mResourceManager.RegisterResourceType(new Texture2DResourceLoader(mRenderSystem, mFileSystem), ".png");
	mResourceManager.RegisterResourceType(new WavefrontResourceLoader(mRenderSystem, mResourceManager, mFileSystem), ".obj");
	//mResourceManager.RegisterResourceType(std::auto_ptr<IResourceLoader>(new ImageMapResourceLoader(*resourceManager)), ".imagemap");

	int32 width = mConfiguration->FindInt("window.width");
	int32 height = mConfiguration->FindInt("window.height");
	std::string title = mConfiguration->FindString("window.title");

	mWindow.Resize(width, height);
	mWindow.SetTitle(title);
	mWindow.AddWindowClosedListener(this);
	
	return true;
}

void GameRunner::Release()
{
	mWindow.RemoveWindowClosedListener(this);
}

bool GameRunner::OnWindowClosing(IWindow& window)
{
	mRunning = false;
	return true;
}

//
// Script integration
//

namespace playstate
{
	int Game_Start(lua_State* L)
	{
		int configRef = luaL_ref(L, LUA_REGISTRYINDEX);
		ScriptedConfiguration* configuration = new ScriptedConfiguration(new ScriptCollection(L, configRef));

		ScriptableGame* game = luaM_getobject<ScriptableGame>(L);
		if(game != NULL) {
			GameRunner* runner = new GameRunner();
			runner->Start(game, configuration);

			delete game;
			delete configuration;
			delete runner;
		}

		return 0;
	}

	int Game_LoadLevel(lua_State* L)
	{
		const std::string levelFileName = lua_tostring(L, -1); lua_pop(L, 1);
		SceneGroup* group = GameRunner::Get().LoadLevel(levelFileName);
		if(group != NULL) {
			luaM_pushobject(L, "SceneGroup", group);
		} else {
			lua_pushnil(L);
		}

		return 1;
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
}