#pragma once
#include "scene_group.h"
#include "../linked_list.h"
#include "../camera/camera.h"

namespace playstate
{
	//
	// There is only one scene for the entire game. The tree-structure looks like this: Scene ->* SceneGroup ->* SceneNode.
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
		// Fire an event receivable from this groups child nodes. Useful for sending global events.
		//
		// @param typeID A unique event type ID
		// @param messageID A unique message ID for the current type ID
		void FireEvent(uint32 typeID, uint32 messageID);
		
		//
		// Fire an event receivable from this groups child nodes. Useful for sending global events.
		//
		// @param typeID A unique event type ID
		// @param messageID A unique message ID for the current type ID
		// @param typeMask Only nodes of a specific type receives this event
		void FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask);

		//
		// Query this scene for items located in it.
		//
		// @param query The query we are performing
		// @param target Container where all the found items are put into
		// {@code
		//	FindQuery query;
		//	query.Camera = &myCamera;
		//	RenderBlockResultSet resultSet;
		//	if(scene.Find(query, &resultSet)) {
		//		
		//	}
		// }
		bool Find(const FindQuery& query, RenderBlockResultSet* target);

		//
		// Query this scene for items located in it and sorts them using the supplied sorter algorithm
		//
		// @param query
		// @param target
		// @param sorter
		bool Find(const FindQuery& query, RenderBlockResultSet* target, IArraySorter<RenderBlock*>* sorter);

		//
		// Queries this scene for light sources, based on the supplied query. The result of the found light 
		// sources are put in the supplied target.
		// 
		// @param query
		// @param target
		bool Find(const FindQuery& query, LightSourceResultSet* target);

		//
		// Sets the ambient lighting on the entire scene. 
		//
		// @param color The color of the ambient light.
		void SetAmbientLight(const Color& color);

		//
		// Retrieves the ambient lighting used on the entire scene.
		//
		// @return The ambient color
		inline const Color& GetAmbientLight() const {
			return mAmbientLight;
		}

		//
		// @return 
		inline Camera& GetActiveCamera() {
			return mCamera;
		}

		//
		// @return
		inline const Camera& GetActiveCamera() const {
			return mCamera;
		}
		

	private:
		LinkedList<SceneGroup> mSceneGroups;
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
	extern int Scene_FireEvent(lua_State* L);
	static luaL_Reg Scene_Methods[] = {
		{ "AddSceneGroup", Scene_AddSceneGroup },
		{ "RemoveSceneGroup", Scene_RemoveSceneGroup },
		{ "SetAmbientLight", Scene_SetAmbientLight },
		{ "GetAmbientLight", Scene_GetAmbientLight },
		{ "FireEvent", Scene_FireEvent },
		{ NULL, NULL }
	};
}
