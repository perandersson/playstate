#pragma once
#include "../linked_list.h"
#include "../types.h"
#include "../script/scriptable.h"
#include "renderable.h"
#include "updatable.h"
#include "tickable.h"
#include "collidable.h"
#include "../scene/scene_node.h"
#include "../scene/scene_group.h"

namespace playstate
{
	//
	// Base class for components in the game engine. 
	class Component : public Scriptable, public Updatable, public Tickable, public Renderable, public Collidable
	{
	public:
		class Features
		{
		public:
			enum {
				UPDATABLE = BIT(0), 
				TICKABLE = BIT(1),
				RENDERABLE = BIT(2),
				COLLIDABLE = BIT(3)
			};
		};

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

	public:

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

		//
		// Set which features to be enabled for this component
		//
		// @param features A bit mask with all features to be enabled. This will also disable
		//	features if those were enabled before but are no longer supplied here. See {@code playstate::Component::Features}
		void SetFeatures(type_mask features);

		//
		// Enables one features for this component
		//
		// @param feature Containing the features needed by this component. See: {@code playstate::Component::Features}
		void EnableFeature(type_mask feature);

		//
		// Disables one features for this component
		//
		// @param feature Containing which features to be disabled. See: {@code playstate::Component::Features}
		void DisableFeature(type_mask feature);

		//
		// @return What features are enabled on this component
		inline type_mask GetFeatures() const {
			return mFeatures;
		}

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

	// Collidable
	public:	
		virtual void CollidesWith(ICollidable* otherObject, const Vector3& direction);

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

	private:
		void EnableFeatures();
		void DisableFeatures();

	protected:
		SceneGroup* mGroup;
		SceneNode* mNode;
		type_mask mTypeMask;
		type_mask mFeatures;

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

