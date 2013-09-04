#pragma once
#include "../script/scriptable.h"
#include "scene_node.h"
#include "../linked_list.h"
#include "../processor/update_processor_factory.h"
#include "../processor/render_processor_factory.h"
#include "../processor/light_source_processor_factory.h"

namespace playstate
{
	//
	// A SceneGroup is the concept where we group scene nodes together in a logical structure. A scene group can
	// be a level, where all the contained nodes are items for that specific level.
	class SceneGroup : public Scriptable, public IUpdateProcessor, public IRenderProcessor, public ILightSourceProcessor
	{
	public:
		LinkedListLink<SceneGroup> GroupLink;

	public:
		SceneGroup();
		SceneGroup(IUpdateProcessor* updateProcessor, IRenderProcessor* renderProcessor, 
			ILightSourceProcessor* lightSourceProcessor);
		virtual ~SceneGroup();

		//
		// Adds the supplied scene node to this scene group instance
		// 
		// @param node The node we want to add
		void AddNode(SceneNode* node);

		//
		// Removes the supplied scene node from this scene group instance
		//
		// @param node The node we want to remove
		void RemoveNode(SceneNode* node);
		
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

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	// ILightSourceProcessor
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const;

	private:
		LinkedList<SceneNode> mSceneNodes;
		IUpdateProcessor* mUpdateProcessor;
		IRenderProcessor* mRenderProcessor;
		ILightSourceProcessor* mLightSourceProcessor;

		// Use this to prevent compontent from deleting this group
		bool mUpdating;
	};
	
	//
	// Script integration
	//

	extern int SceneGroup_Factory(lua_State* L);
	extern int SceneGroup_Init(lua_State* L);
	extern int SceneGroup_Load(lua_State* L);
	extern int SceneGroup_AddNode(lua_State* L);
	extern int SceneGroup_RemoveNode(lua_State* L);
	extern int SceneGroup_FireEvent(lua_State* L);
	static luaL_Reg SceneGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneGroup_Factory },
		{ LUA_INHERIT_CONSTRUCTOR, SceneGroup_Init },
		{ "Load", SceneGroup_Load },
		{ "AddNode", SceneGroup_AddNode },
		{ "RemoveNode", SceneGroup_RemoveNode },
		{ "FireEvent", SceneGroup_FireEvent },
		{ NULL, NULL }
	};
}