#pragma once
#include "../linked_list.h"
#include "../types.h"
#include "../script/scriptable.h"

namespace playstate
{
	class SceneNode;

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
		// @param type The component type. 
		//		If you want to be able to retireve a component from another component then you have to
		//		supply a component type ID here. Any value larger then 0 is fine.
		Component(uint32 typeMask);

		//
		// Destructor
		virtual ~Component();

	public:
		//
		// 
		void OnAddedToSceneNode(SceneNode* node);

	public:
		// Read-only property for the owner scene node
		SceneNode*& const Owner;

		// 
		const uint32& Type;

	protected:		
		//
		// Method called when this component is added to a scene node.
		virtual void OnComponentAdded();

		//
		// Method called when this component is removed from a scene node.
		virtual void OnComponentRemoved();

	protected:
		SceneNode* mOwner;
		uint32 mType;
	};
}

