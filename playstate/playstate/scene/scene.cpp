#include "../memory/memory.h"
#include "scene.h"
#include "../game/game_runner.h"
#include "../search/sorter/render_block_array_sorter.h"
using namespace playstate;

Scene::Scene() : mSceneGroups(offsetof(SceneGroup, GroupLink))
{
}

Scene::~Scene()
{
	mSceneGroups.DeleteAll();
}

void Scene::AddSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	mSceneGroups.AddLast(group);
}
		
void Scene::RemoveSceneGroup(SceneGroup* group)
{
	assert_not_null(group);
	delete group; // This will unlink it as well!
}

void Scene::Update()
{
	SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		SceneGroup* next = group->GroupLink.Tail;
		group->Update();
		group = next;
	}
}

void Scene::FireEvent(uint32 typeID, uint32 messageID)
{
	SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		SceneGroup* next = group->GroupLink.Tail;
		group->FireEvent(typeID, messageID);
		group = next;
	}
}

void Scene::FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask)
{
	SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		SceneGroup* next = group->GroupLink.Tail;
		group->FireEvent(typeID, messageID, typeMask);
		group = next;
	}
}

bool Scene::Find(const FindQuery& query, RenderBlockResultSet* target)
{
	RenderBlockArraySorter defaultSorter;
	return Find(query, target, &defaultSorter);
}

bool Scene::Find(const FindQuery& query, LightSourceResultSet* target)
{
	bool found = false;
	const SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		const SceneGroup* next = group->GroupLink.Tail;
		if(group->Find(query, target))
			found = true;
		group = next;
	}
	return found;
}

bool Scene::Find(const FindQuery& query, RenderBlockResultSet* target, IArraySorter<RenderBlock*>* sorter)
{
	bool found = false;
	const SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		const SceneGroup* next = group->GroupLink.Tail;
		if(group->Find(query, target))
			found = true;
		group = next;
	}
	target->Sort(sorter);
	return found;
}

void Scene::SetAmbientLight(const Color& color)
{
	mAmbientLight = color;
}

namespace playstate
{
	int Scene_AddSceneGroup(lua_State* L)
	{
		SceneGroup* sceneGroup = luaM_popobject<SceneGroup>(L);
		if(sceneGroup != NULL)
			GameRunner::Get().GetScene().AddSceneGroup(sceneGroup);
		else
			ILogger::Get().Error("Cannot add a SceneGroup that's nil");

		return 0;
	}

	int Scene_RemoveSceneGroup(lua_State* L)
	{
		SceneGroup* sceneGroup = luaM_popobject<SceneGroup>(L);
		if(sceneGroup != NULL)
			GameRunner::Get().GetScene().RemoveSceneGroup(sceneGroup);
		else
			ILogger::Get().Error("Cannot remove a SceneGroup that's nil");

		return 0;
	}
	
	int Scene_SetAmbientLight(lua_State* L)
	{
		Color ambientColor = luaM_popcolor(L);
		GameRunner::Get().GetScene().SetAmbientLight(ambientColor);
		return 0;
	}
	
	int Scene_GetAmbientLight(lua_State* L)
	{
		luaM_pushcolor(L, GameRunner::Get().GetScene().GetAmbientLight());
		return 1;
	}
	
	int Scene_FireEvent(lua_State* L)
	{
		if(lua_gettop(L) < 2) {
			luaM_printerror(L, "Expected: Scene.FireEvent(typeID, messageID)");
			lua_pushnil(L);
			return 1;
		}
		
		uint32 messageID = lua_tointeger(L, -1); lua_pop(L, 1);
		uint32 typeID = lua_tointeger(L, -1); lua_pop(L, 1);
		GameRunner::Get().GetScene().FireEvent(typeID, messageID);
		return 0;
	}
}
