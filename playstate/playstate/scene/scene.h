#pragma once
#include "scene_group.h"
#include "../linked_list.h"
#include "../camera/camera.h"

namespace playstate
{
	class Scene
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
		bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

		//
		// Queries the scene for items and sorts them using the supplied sorter algorithm
		//bool Find(const FindQuery& query, RenderBlockResultSet* target, IArraySorter<uint32>* sorter) const;

		//
		// Query the scene for lights located in it
		// @param query
		// @param target
		//bool Find(const FindQuery& query, std::vector<Light*>* target) const;

		//
		// Sets the ambient lighting on the entire scene. 
		// @param color The color of the ambient light.
		void SetAmbientLight(const Color& color);

	public:
		Camera& const ActiveCamera;

		// Read-only property for the color of the ambient light.
		Color& const AmbientLight;

	private:
		LinkedList<SceneGroup, &SceneGroup::GroupLink> mSceneGroups;
		Camera mCamera;
		Color mAmbientLight;
	};
	
	//
	// Script integration
	//

	extern int Scene_AddSceneGroup(lua_State* L);
	extern int Scene_RemoveSceneGroup(lua_State* L);
	extern int Scene_SetAmbientLight(lua_State* L);
	extern int Scene_GetAmbientLight(lua_State* L);
	static luaL_Reg Scene_Methods[] = {
		{ "AddSceneGroup", Scene_AddSceneGroup },
		{ "RemoveSceneGroup", Scene_RemoveSceneGroup },
		{ "SetAmbientLight", Scene_SetAmbientLight },
		{ "GetAmbientLight", Scene_GetAmbientLight },
		{ NULL, NULL }
	};
}
