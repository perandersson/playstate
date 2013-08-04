#pragma once

#include "../script/scriptable.h"
#include "../window/window.h"
#include "../scene/scene.h"
#include "configuration.h"

namespace playstate
{
	//
	class IGame
	{
	public:
		virtual ~IGame() {}

	public:
		//
		// Initializes the game's internal resources.
		// @return TRUE if the initialization was successfull; FALSE otherwise
		virtual bool Initialize() = 0;

		//
		// Releases the game's internal resources.
		virtual void Release() = 0;

		//
		// Loads content
		virtual void LoadContent() = 0;

		//
		// Unloads content
		virtual void UnloadContent() = 0;

		//
		// Updates the game
		virtual void Update() = 0;
	};

	//
	// Implementation used if the user decides to create the Game class using script.
	class ScriptableGame : public IGame, public Scriptable
	{
	public:
		ScriptableGame();
		virtual ~ScriptableGame();

	// IGame
	public:
		virtual bool Initialize();
		virtual void Release();
		virtual void LoadContent();
		virtual void UnloadContent();
		virtual void Update();

	// Scriptable
	public:
		virtual void OnRegistered();

	private:
		uint32 mLoadContentFunc;
		uint32 mUnloadContentFunc;
		uint32 mInitializeFunc;
		uint32 mReleaseFunc;
		uint32 mUpdateFunc;
	};

	//
	// Script integration
	//

	extern int IGame_Init(lua_State* L);
	static luaL_Reg IGame_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, IGame_Init },
		{ NULL, NULL }
	};
}
