#pragma once
#include "../linked_list.h"
#include "../types.h"
#include "../script/scriptable.h"
#include "../processor/renderable.h"
#include "../processor/updatable.h"
#include "../processor/tickable.h"

namespace playstate
{
	class SceneNode;
	class SceneGroup;

	//
	// Base class for components in the game engine. 
	class Component : public Scriptable, public Updatable, public Tickable
	{
	public:
		LinkedListLink<Component> ComponentLink;

	public:
		//
		// Constructor
		Component();

		//
		// Constructor
		// @param typeMask The component type. 
		//		If you want to be able to retireve a component from another component then you have to
		//		supply a component type ID here. Any value larger then 0 is fine.
		Component(type_mask typeMask);

		//
		// Destructor
		virtual ~Component();

	public:
		//
		// 
		void OnAttachedToSceneNode(SceneNode* node);

		//
		// 
		void OnAttachedToSceneGroup(SceneGroup* group);

		//
		//
		void OnDetachingFromSceneNode(SceneNode* node);
		
		//
		// 
		void OnDetachingFromSceneGroup(SceneGroup* group);

		//
		// @return The owner scene node for this component
		SceneNode* GetNode() const;

		//
		// @return The owners group for this component
		SceneGroup* GetGroup() const;

		//
		// @return This componenents type mask.
		type_mask GetTypeMask() const;

		//
		// Sets the component type.
		void SetTypeMask(type_mask typeMask);

		//
		// Notifies this component that a new potential event has been triggered
		void NotifyOnEvent(uint32 typeID, uint32 messageID);

	// Updatable
	public:
		virtual void Update();

	// Tickable
	public:
		virtual void Tick();
		
	// Scriptable
	public:
		virtual void OnRegistered();
		
	// Renderable
	public:
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet);
		virtual const AABB& GetBoundingBox() const;
		virtual ISpatialTree* GetTree();

	protected:		
		//
		// Method called when this component is added to a scene node.
		// It's from this method that we attach this to the available processors in the game engine.
		// One such functionality is the {@code playstate::Updatable} class.
		virtual void OnComponentAdded();

		//
		// Method called when this component is removed from a scene node.
		virtual void OnComponentRemoved();

		//
		// Method called when this component receives events from the game engine.
		virtual void OnEvent(uint32 typeID, uint32 messageID);

	protected:
		SceneGroup* mGroup;
		SceneNode* mNode;
		type_mask mTypeMask;

	private:
		ScriptMethod* mOnComponentAdded;
		ScriptMethod* mOnComponentRemoved;
		ScriptMethod* mOnEvent;
		ScriptMethod* mUpdate;
		ScriptMethod* mTick;
	};
	
	extern int Component_Init(lua_State* L);
	extern int Component_GetNode(lua_State* L);
	extern int Component_TranslateNode(lua_State* L);
	extern int Component_SetNodePosition(lua_State* L);
	extern int Component_SetNodeRotation(lua_State* L);
	extern int Component_GetNodePosition(lua_State* L);
	//extern int Component_Show(lua_State* L);
	//extern int Component_Hide(lua_State* L);
	extern int Component_FireEvent(lua_State* L);
	static luaL_Reg Component_Methods[] = {
		{ LUA_INHERIT_CONSTRUCTOR, Component_Init },
		{ "GetNode", Component_GetNode },
		{ "TranslateNode", Component_TranslateNode },
		{ "SetNodePosition", Component_SetNodePosition },
		{ "SetNodeRotation", Component_SetNodeRotation },
		{ "GetNodePosition", Component_GetNodePosition },
		//{ "Show", Component_Show },
		//{ "Hide", Component_Hide },
		{ "FireEvent", Component_FireEvent },
		{ NULL, NULL }
	};
}

