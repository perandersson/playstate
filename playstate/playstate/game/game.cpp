#include "../memory/memory.h"
#include "game.h"
#include "scripted_configuration.h"
#include "../script/script_system.h"
using namespace playstate;

ScriptableGame::ScriptableGame()
	: mLoadContent(NULL), mUnloadContent(NULL), mInitialize(NULL), mRelease(NULL), mUpdate(NULL)
{
}

ScriptableGame::~ScriptableGame()
{
	delete mLoadContent;
	delete mUnloadContent;
	delete mInitialize;
	delete mRelease;
	delete mUpdate;
}

void ScriptableGame::LoadContent()
{
	if(mLoadContent != NULL)
		mLoadContent->Invoke();
}

void ScriptableGame::UnloadContent()
{
	if(mUnloadContent != NULL)
		mUnloadContent->Invoke();
}

bool ScriptableGame::Initialize()
{
	if(mInitialize != NULL) {
		if(mInitialize->Invoke()) {
			return mInitialize->GetBool();
		}
		return false;
	}
	return true;
}

void ScriptableGame::Release()
{
	if(mRelease != NULL)
		mRelease->Invoke();
}

void ScriptableGame::Update()
{
	if(mUpdate != NULL)
		mUpdate->Invoke();
}

void ScriptableGame::OnRegistered()
{
	mLoadContent = GetMethod("LoadContent");
	mUnloadContent = GetMethod("UnloadContent");
	mInitialize = GetMethod("Initialize");
	mRelease = GetMethod("Release");
	mUpdate = GetMethod("Update");
}

//
// Script integration
//

namespace playstate
{
	int IGame_Init(lua_State* L)
	{
		if(lua_istable(L, -1) == 0) {
			lua_pop(L, 1);
			return 0;
		}

		ScriptableGame* game = new ScriptableGame();
		luaM_setinstance(L, game);
		
		const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		game->RegisterObject(L, ref);
		return 0;
	}
}