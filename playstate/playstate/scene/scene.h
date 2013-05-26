#pragma once
#include "../script/scriptable.h"
#include "scene_group.h"
#include "../linked_list.h"
#include "../camera/camera.h"

namespace playstate
{
	class Scene : public Scriptable
	{
	public:
		Scene();
		~Scene();
		
		//
		// Adds a new scene group to the current scene.
		void AddSceneGroup(SceneGroup* group);
		
		//
		// Removes a scene group to the current scene.
		void RemoveSceneGroup(SceneGroup* group);
		
		//
		// Update the scene
		void Update();

		//
		// Query the Scene for items located in it.
		// @param query The query we are performing
		// @param target Container where all the found items are put into
		bool Find(const FindQuery& query, FindResultSet<RenderBlock>* target) const;

		//
		// Query the scene for lights located in it
		// @param query
		// @param target
		//bool QueryLights(const FindQuery& query, std::vector<Light*>* target) const;

	public:
		Camera& const ActiveCamera;

	private:
		LinkedList<SceneGroup, &SceneGroup::GroupLink> mSceneGroups;
		Camera mCamera;
	};
	
	//
	// Script integration
	//

	extern int Scene_AddSceneGroup(lua_State* L);
	extern int Scene_RemoveSceneGroup(lua_State* L);
	static luaL_Reg Scene_Methods[] = {
		{ "AddSceneGroup", Scene_AddSceneGroup },
		{ "RemoveSceneGroup", Scene_RemoveSceneGroup },
		{ NULL, NULL }
	};
}
