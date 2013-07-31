#include "../memory/memory.h"
#include "../kernel.h"
#include "game_runner.h"
#include "scripted_configuration.h"
#include "../model/wavefront/wavefront_resource_loader.h"
#include "../rendering/graphics_driver.h"
#include "../input/input_system.h"
using namespace playstate;

namespace playstate
{
	float32 GameDeltaTime = 0.0f;
	float64 GameTotalTime = 0.0f;
}

template<> playstate::GameRunner* playstate::Singleton<playstate::GameRunner>::gSingleton = NULL;

GameRunner::GameRunner(IGame* game, IConfiguration* configuration) 
	: mGame(game), mConfiguration(configuration), mRenderPipeline(NULL), mRunning(true)
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

	mGame->UnloadContent();

	mGame->Release();
	Release();
}

void GameRunner::StartLevel(const std::string& level)
{
	ScriptSystem& scriptSystem = ScriptSystem::Get();

	std::auto_ptr<Script> script = scriptSystem.CompileFile(level);
	SceneGroup* group = script->ReadInstance<SceneGroup>();
	mScene.AddSceneGroup(group);

	IKernel& kernel = IKernel::Get();
	IWindow& window = IWindow::Get();
	IRenderContext* screenRenderContext = IGraphicsDriver::Get().GetScreenRenderContext();

	while(mRunning) {
		GameDeltaTime = window.GetTimeSinceLastUpdate();
		GameTotalTime += (float64)GameDeltaTime;

		scriptSystem.SetGlobalVar("GameDeltaTime", GameDeltaTime);
		scriptSystem.SetGlobalVar("GameTotalTime", GameTotalTime);

		mScene.Update();
		mGame->Update();
		mRenderPipeline->Render(mScene, mScene.GetActiveCamera());
		mGame->Render();
		
		screenRenderContext->SwapBuffers();
		kernel.Process();
	}
}

SceneGroup* GameRunner::LoadLevel(const std::string& fileName)
{
	try {
		ScriptSystem& scriptSystem = ScriptSystem::Get();
		std::auto_ptr<Script> script = scriptSystem.CompileFile(fileName);
		SceneGroup* grp = script->ReadInstance<SceneGroup>();
		return grp;
	} catch(ScriptException e) {
		// TODO: Add logging
	}
	return NULL;
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
	ResourceManager& resourceManager = ResourceManager::Get();
	IFileSystem& fileSystem = IFileSystem::Get();
	RenderSystem& renderSystem = RenderSystem::Get();
	IWindow& window = IWindow::Get();

	// Register resource types
	resourceManager.RegisterResourceType(new Texture2DResourceLoader(renderSystem, fileSystem), ".png");
	resourceManager.RegisterResourceType(new WavefrontResourceLoader(resourceManager, fileSystem), ".obj");

	int32 width = mConfiguration->FindInt("window.width");
	int32 height = mConfiguration->FindInt("window.height");
	std::string title = mConfiguration->FindString("window.title");

	window.Resize(width, height);
	window.SetTitle(title);
	window.AddWindowClosedListener(this);
	
	return true;
}

void GameRunner::Release()
{
	IWindow& window = IWindow::Get();
	window.RemoveWindowClosedListener(this);
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
			GameRunner runner(game, configuration);
			runner.Start();

			delete game;
			delete configuration;
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

	int Game_StartLevel(lua_State* L)
	{
		const std::string levelFileName = lua_tostring(L, -1); lua_pop(L, 1);
		GameRunner::Get().StartLevel(levelFileName);
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
}