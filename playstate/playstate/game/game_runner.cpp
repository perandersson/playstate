#include "../memory/memory.h"
#include "game_runner.h"
#include "scripted_configuration.h"
#include "../model/wavefront/wavefront_resource_loader.h"
#include "../rendering/graphics_driver.h"
#include "../input/input_system.h"
using namespace playstate;

namespace playstate
{
	float32 _GameDeltaTime = 0.0f;
	float64 _GameTotalTime = 0.0f;

	const float32& GameDeltaTime = _GameDeltaTime;
	const float64& GameTotalTime = _GameTotalTime;
}

template<> playstate::GameRunner* playstate::Singleton<playstate::GameRunner>::gSingleton = NULL;

GameRunner::GameRunner(IWindow& window, IFileSystem& fileSystem, ScriptSystem& scriptSystem,
	RenderSystem& renderSystem, ResourceManager& resourceManager, IInputSystem& inputSystem,
	IGame* game, IConfiguration* configuration) 
	: mWindow(window), mFileSystem(fileSystem), mScriptSystem(scriptSystem),
	mRenderSystem(renderSystem), mResourceManager(resourceManager), mInputSystem(inputSystem),
	mGame(game), mConfiguration(configuration), mRenderPipeline(NULL), mRunning(true),
	mScreenRenderContext(IGraphicsDriver::Get().ScreenRenderContext),
	ActiveScene(mScene)
{
	assert_not_null(game);
	assert_not_null(configuration);
}

GameRunner::~GameRunner()
{
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

	while(mRunning) {
		_GameDeltaTime = mWindow.GetTimeSinceLastUpdate();
		_GameTotalTime += (float64)GameDeltaTime;

		mScriptSystem.SetGlobalVar("GameDeltaTime", GameDeltaTime);
		mScriptSystem.SetGlobalVar("GameTotalTime", GameTotalTime);

		mScene.Update();
		mGame->Update();
		mRenderPipeline->Render(mScene, mScene.ActiveCamera);
		mGame->Render();
		
		mScreenRenderContext->SwapBuffers();
		mWindow.HandleEvents();
		mInputSystem.Poll();
		mResourceManager.Poll();
	}

	mGame->UnloadContent();

	mGame->Release();
	Release();
}

SceneGroup* GameRunner::LoadLevel(const std::string& fileName)
{
	std::auto_ptr<Script> script = mScriptSystem.CompileFile(fileName);
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
	mResourceManager.RegisterResourceType(new WavefrontResourceLoader(mResourceManager, mFileSystem), ".obj");

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
			GameRunner* runner = new GameRunner(IWindow::Get(), IFileSystem::Get(), ScriptSystem::Get(), 
				RenderSystem::Get(), ResourceManager::Get(), IInputSystem::Get(), game, configuration);
			runner->Start();

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