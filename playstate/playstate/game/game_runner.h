#pragma once

#include "../singleton.h"
#include "game.h"
#include "../scene/scene.h"
#include "../gui/canvas.h"
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
	class GameRunner : public Singleton<GameRunner>, public IWindowClosedListener
	{
	public:
		GameRunner(IGame* game, IConfiguration* configuration);
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
		// Loads the supplied interface and returns it as a canvas group.
		// @param fileName
		CanvasGroup* LoadUserInterface(const std::string& fileName);

		//
		// Defines how the scene should be rendered to the screen using the active camera.
		// 
		// @param renderPipeline
		void SetRenderPipeline(IRenderPipeline* renderPipeline);

		//
		// @return This games scene. Each game has only one scene. The scene can contain multiple scene groups - which can be 
		//	seen as the level of the game.
		Scene& GetScene();
		const Scene& GetScene() const;

		Canvas& GetCanvas();
		const Canvas& GetCanvas() const;

	// IWindowClosedListener
	public:
		virtual bool OnWindowClosing();

	private:
		bool Initialize();
		void Run();
		void Release();

	private:
		IGame* mGame;
		IConfiguration* mConfiguration;
		IRenderPipeline* mRenderPipeline;
		Scene mScene;
		Canvas mCanvas;
		bool mRunning;
	};

	
	//
	// Script integration
	//
	
	extern int Game_LoadLevel(lua_State* L);
	extern int Game_StartLevel(lua_State* L);
	extern int Game_Start(lua_State* L);
	extern int Game_SetRenderPipeline(lua_State* L);
	extern int Game_LoadUserInterface(lua_State* L);
	static luaL_Reg Game_Methods[] = {
		{ "LoadLevel", Game_LoadLevel },
		{ "StartLevel", Game_StartLevel },
		{ "Start", Game_Start },
		{ "SetRenderPipeline", Game_SetRenderPipeline },
		{ "LoadUserInterface", Game_LoadUserInterface },
		{ NULL, NULL }
	};
}
