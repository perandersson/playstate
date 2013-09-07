#pragma once
#include "../linked_list.h"
#include "../types.h"
#include "../script/scriptable.h"

namespace playstate
{
	class SceneNode;
	class SceneGroup;

	//
	// Base class for components in the game engine. 
	// Example of a component is the RenderStaticModel class.
	class Component
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

	protected:		
		//
		// Method called when this component is added to a scene node.
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
	};
}

