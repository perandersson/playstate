#pragma once

#include "../singleton.h"
#include "game.h"
#include "../scene/scene.h"
#include "../window/window.h"
#include "../script/script_system.h"
#include "../filesystem/file_system.h"
#include "../rendering/render_system.h"
#include "../rendering/render_pipeline.h"
#include "../resources/resource_manager.h"

namespace playstate
{
	class IInputSystem;

	//
	// The main application for the game engine
	class GameRunner : public Singleton<GameRunner>, public IWindowClosedListener
	{
	public:
		GameRunner(IWindow& window, IFileSystem& fileSystem, ScriptSystem& scriptSystem,
			RenderSystem& renderSystem, ResourceManager& resourceManager, IInputSystem& inputSystem,
			IGame* game, IConfiguration* configuration);
		~GameRunner();

		//
		// Starts the supplied game using the supplied configuration
		void Start();

		// Loads a new level and returns the level as a SceneGroup type.
		// @param fileName 
		// @return
		SceneGroup* LoadLevel(const std::string& fileName);

		//
		// Starts a new level. This also starts the main game loop
		// @param fileName
		void StartLevel(const std::string& fileName);

		//
		//
		void SetRenderPipeline(IRenderPipeline* renderPipeline);

	public:
		// Read-only property for the active scene
		Scene& const ActiveScene; 

	// IWindowClosedListener
	public:
		virtual bool OnWindowClosing(IWindow& window);

	private:
		bool Initialize();
		void Release();

	private:
		IWindow& mWindow;
		IFileSystem& mFileSystem;
		ScriptSystem& mScriptSystem;
		RenderSystem& mRenderSystem;
		ResourceManager& mResourceManager;
		IInputSystem& mInputSystem;

	private:
		IGame* mGame;
		IConfiguration* mConfiguration;
		IRenderPipeline* mRenderPipeline;
		IRenderContext* mScreenRenderContext;
		Scene mScene;
		bool mRunning;
	};

	
	//
	// Script integration
	//
	
	extern int Game_LoadLevel(lua_State* L);
	extern int Game_StartLevel(lua_State* L);
	extern int Game_Start(lua_State* L);
	extern int Game_SetRenderPipeline(lua_State* L);
	static luaL_Reg Game_Methods[] = {
		{ "LoadLevel", Game_LoadLevel },
		{ "StartLevel", Game_StartLevel },
		{ "Start", Game_Start },
		{ "SetRenderPipeline", Game_SetRenderPipeline },
		{ NULL, NULL }
	};
}
