#include "../memory/memory.h"
#include "game.h"
#include "scripted_configuration.h"
#include "../script/script_system.h"
using namespace playstate;

ScriptableGame::ScriptableGame()
	: mLoadContentFunc(0), mUnloadContentFunc(0), mInitializeFunc(0), mReleaseFunc(0)
{
}

ScriptableGame::~ScriptableGame()
{
}

void ScriptableGame::LoadContent()
{
	if(mLoadContentFunc != 0 && PrepareMethod(mLoadContentFunc)) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableGame::UnloadContent()
{
	if(mUnloadContentFunc != 0 && PrepareMethod(mUnloadContentFunc)) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

bool ScriptableGame::Initialize()
{
	if(mInitializeFunc != 0) {
		if(PrepareMethod(mInitializeFunc)) {
			if(lua_pcall(mCurrentState, 1, 1, NULL) == 0) {
				bool res = lua_toboolean(mCurrentState, -1) == 1; lua_pop(mCurrentState, 1);
				return res;
			} else {
				const char* err = lua_tostring(mCurrentState, -1);
				lua_pop(mCurrentState, 1);
			}
		}
		return false;
	}
	return true;
}

void ScriptableGame::Release()
{
	if(mReleaseFunc != 0 && PrepareMethod(mReleaseFunc)) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableGame::Update()
{
	if(mUpdateFunc != 0 && PrepareMethod(mUpdateFunc)) {
		if(lua_pcall(mCurrentState, 1, 0, NULL) == 0) {
		} else {
			const char* err = lua_tostring(mCurrentState, -1);
			lua_pop(mCurrentState, 1);
		}
	}
}

void ScriptableGame::OnRegistered()
{
	mLoadContentFunc = GetMethodID("LoadContent");
	mUnloadContentFunc = GetMethodID("UnloadContent");
	mInitializeFunc = GetMethodID("Initialize");
	mReleaseFunc = GetMethodID("Release");
	mUpdateFunc = GetMethodID("Update");
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