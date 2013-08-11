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
		void OnAttachedToScene(SceneNode* node);

		//
		//
		void OnDetachingFromScene(SceneNode* node);

		//
		// @return The owner scene node for this component
		SceneNode* GetNode() const;

		//
		// @return This componenents type mask.
		type_mask GetTypeMask() const;

		//
		// Sets the component type.
		void SetTypeMask(type_mask typeMask);

	protected:		
		//
		// Method called when this component is added to a scene node.
		virtual void OnComponentAdded();

		//
		// Method called when this component is removed from a scene node.
		virtual void OnComponentRemoved();

	protected:
		SceneNode* mNode;
		type_mask mTypeMask;
	};
}

