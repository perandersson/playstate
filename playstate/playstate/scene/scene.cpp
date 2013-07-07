#include "../memory/memory.h"
#include "scene.h"
#include "../game/game_runner.h"
#include "../search/sorter/render_block_array_sorter.h"
using namespace playstate;

Scene::Scene()
	: ActiveCamera(mCamera), AmbientLight(mAmbientLight)
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


bool Scene::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	bool found = false;
	const SceneGroup* group = mSceneGroups.First();
	while(group != NULL) {
		const SceneGroup* next = group->GroupLink.Tail;
		if(group->Find(query, target))
			found = true;
		group = next;
	}
	RenderBlockArraySorter defaultSorter;
	target->Sort(&defaultSorter);
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
		if(sceneGroup != NULL) {
			GameRunner::Get().ActiveScene.AddSceneGroup(sceneGroup);
		}

		return 0;
	}

	int Scene_RemoveSceneGroup(lua_State* L)
	{
		SceneGroup* sceneGroup = luaM_popobject<SceneGroup>(L);
		if(sceneGroup != NULL) {
			GameRunner::Get().ActiveScene.RemoveSceneGroup(sceneGroup);
		}

		return 0;
	}
	
	int Scene_SetAmbientLight(lua_State* L)
	{
		Color ambientColor = luaM_popcolor(L);
		GameRunner::Get().ActiveScene.SetAmbientLight(ambientColor);
		return 0;
	}
}
