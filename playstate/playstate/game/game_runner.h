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
	//
	// The main application for the game engine
	// TODO: Move required parameters to constructor (i.e. game and configuration).
	class GameRunner : public Singleton<GameRunner>, public IWindowClosedListener
	{
	public:
		GameRunner();
		~GameRunner();

		//
		// Starts the supplied game using the supplied configuration
		void Start(IGame* game, IConfiguration* configuration);

		//
		// @param 
		// @return
		SceneGroup* LoadLevel(const std::string& fileName);

		//
		//
		void SetRenderPipeline(IRenderPipeline* renderPipeline);

	// IWindowClosedListener
	public:
		virtual bool OnWindowClosing(IWindow& window);

	public:
		// Read-only property for the active scene
		Scene& const ActiveScene;

	private:
		bool Initialize();
		void Release();

	private:
		IWindow& mWindow;
		IFileSystem& mFileSystem;
		ScriptSystem& mScriptSystem;
		RenderSystem& mRenderSystem;
		ResourceManager& mResourceManager;

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
	extern int Game_Start(lua_State* L);
	extern int Game_SetRenderPipeline(lua_State* L);
	static luaL_Reg Game_Methods[] = {
		{ "LoadLevel", Game_LoadLevel },
		{ "Start", Game_Start },
		{ "SetRenderPipeline", Game_SetRenderPipeline },
		{ NULL, NULL }
	};
}
