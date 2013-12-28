#pragma once
#include "../script/scriptable.h"
#include "scene_node.h"
#include "../linked_list.h"
#include "../processor/update_processor.h"
#include "../processor/render_processor.h"
#include "../processor/light_source_processor.h"

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
		SceneGroup(std::auto_ptr<IUpdateProcessor> updateProcessor, std::auto_ptr<IRenderProcessor> renderProcessor, 
			std::auto_ptr<ILightSourceProcessor> lightSourceProcessor);
		virtual ~SceneGroup();
		
		//
		// Add a scene node child for this group
		void AddChild(SceneNode* node);

		//
		// Removes a scene node child from this group
		void RemoveChild(SceneNode* node);

		//
		// Add the supplied component
		// @param component
		void AddComponent(Component* component);

		//
		// Remove the supplied component.
		// @param component
		void RemoveComponent(Component* component);

		//
		// Retrieves a component based on the supplied type. If no component is found then return NULL.
		// @param type
		// @return The component if found; NULL otherwise
		Component* GetComponent(type_mask type);

		//
		// Fire an event receivable from this nodes component. Useful for communication 
		// between components in the same node.
		//
		// @param typeID A unique event type ID
		// @param messageID A unique message ID for the current type ID
		void FireEvent(uint32 typeID, uint32 messageID);
		
		//
		// Fire an event receivable from this nodes component and child nodes. 
		// Useful for sending global events.
		//
		// @param typeID A unique event type ID
		// @param messageID A unique message ID for the current type ID
		// @param typeMask Only nodes of a specific type receives this event
		void FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask);

		//
		// Sets the position of this node.
		void SetPosition(const Vector3& position);

		//
		// @return The position of this scene group
		inline const Vector3& GetPosition() const {
			return mPosition;
		}
		
	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	// IRenderProcessor
	public:
		virtual void AttachRenderable(SceneNode* node);
		virtual void DetachRenderable(SceneNode* node);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	// ILightSourceProcessor
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const;

	private:
		std::auto_ptr<IUpdateProcessor> mUpdateProcessor;
		std::auto_ptr<IRenderProcessor> mRenderProcessor;
		std::auto_ptr<ILightSourceProcessor> mLightSourceProcessor;

		LinkedList<Component> mComponents;
		LinkedList<SceneNode> mChildren;

		Vector3 mPosition;
	};
	
	//
	// Script integration
	//

	extern int SceneGroup_Factory(lua_State* L);
	extern int SceneGroup_Load(lua_State* L);
	extern int SceneGroup_AddChild(lua_State* L);
	extern int SceneGroup_RemoveChild(lua_State* L);
	extern int SceneGroup_FireEvent(lua_State* L);
	extern int SceneGroup_AddComponent(lua_State* L);
	extern int SceneGroup_RemoveComponent(lua_State* L);
	extern int SceneGroup_SetPosition(lua_State* L);
	static luaL_Reg SceneGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneGroup_Factory },
		{ "Load", SceneGroup_Load },
		{ "AddChild", SceneGroup_AddChild },
		{ "RemoveChild", SceneGroup_RemoveChild },
		{ "FireEvent", SceneGroup_FireEvent },
		{ "AddComponent", SceneGroup_AddComponent },
		{ "RemoveComponent", SceneGroup_RemoveComponent },
		{ "SetPosition", SceneGroup_SetPosition },
		{ NULL, NULL }
	};
}