#pragma once
#include "../script/scriptable.h"
#include "scene_node.h"
#include "../linked_list.h"
#include "../processor/update_processor_factory.h"
#include "../processor/render_processor_factory.h"

namespace playstate
{
	//
	// A SceneGroup is the concept where we group scene nodes together in a logical structure. A scene group can
	// be a level, where all the contained nodes are items for that specific level.
	class SceneGroup : public Scriptable, public IUpdateProcessor, public IRenderProcessor
	{
	public:
		LinkedListLink<SceneGroup> GroupLink;

	public:
		SceneGroup();
		virtual ~SceneGroup();

		//
		// 
		void AddSceneNode(SceneNode* node);

		//
		//
		void RemoveSceneNode(SceneNode* node);
		
	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void Update();

	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, FindResultSet<RenderBlock>* target) const;

	private:
		LinkedList<SceneNode, &SceneNode::NodeLink> mSceneNodes;
		IUpdateProcessor* mUpdateProcessor;
		IRenderProcessor* mRenderProcessor;
	};
	
	//
	// Script integration
	//

	extern int SceneGroup_Factory(lua_State* L);
	extern int SceneGroup_Init(lua_State* L);
	extern int SceneGroup_GetID(lua_State* L);
	static luaL_Reg SceneGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneGroup_Factory },
		{ LUA_INHERIT_CONSTRUCTOR, SceneGroup_Init },
		{ LUA_TOSTRING, SceneGroup_GetID },
		{ "GetID", SceneGroup_GetID },
		{ NULL, NULL }
	};
}