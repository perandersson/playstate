#pragma once
#include "../script/scriptable.h"
#include "../linked_list.h"
#include "../component/component.h"
#include "../math/matrix4x4.h"
#include <vector>

namespace playstate
{
	class SceneGroup;

	class SceneNode : public Scriptable
	{
		typedef std::vector<SceneNode*> ChildNodes;

	public:
		LinkedListLink<SceneNode> NodeLink;

	public:
		//
		// Constructor
		// @param group The group where this node is to be placed. This will add the current
		//	instance to the group automatically.
		SceneNode(SceneGroup* group);

		//
		// Constructor
		// @param group The group where this node is to be placed. This will add the current
		//	instance to the group automatically.
		// @param typeMask The scene node type mask
		SceneNode(SceneGroup* group, type_mask typeMask);

		//
		//
		virtual ~SceneNode();
		
	protected:
		//
		// Updates the absolute position value based on the parents position
		void UpdatePosition();

		//
		// Updates the absolute rotation value based on the parents rotation
		void UpdateRotation();

		//
		// Update the model matrix value based on the position and rotation
		void UpdateModelMatrix();

	public:
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
		// Sets the position of this node.
		void SetPosition(const Vector3& position);

		//
		// Sets the rotation of this node. 
		void SetRotation(const Vector3& rotation);
		
		//
		// Remove this item from the scene. 
		// This will also delete this object and clean-up all related resources (child nodes for example).
		void RemoveFromScene();
		
		//
		// Add a node
		void AddChildNode(SceneNode* node);

		//
		// 
		void RemoveChildNode(SceneNode* node);
				
	public:
		// Public read-only property for retrieving the relative rotation of this node.
		const Vector3& Rotation;

		// Public read-only property for reading the absolute rotation of this node.
		const Vector3& AbsoluteRotation;

		// Public read-only property for reading the relative position of this node
		const Vector3& Position;

		// Public read-only property for reading the absolute position of this node
		const Vector3& AbsolutePosition;

		// Public read-only property for reading the absolute model matrix of this node.
		const Matrix4x4& ModelMatrix;
		
		// Read-only property for where this node is contain in.
		SceneGroup*& const Group;

		// Public read-only property for the type mask.
		const type_mask& TypeMask;
		
	private:
		Vector3 mRotation;
		Vector3 mAbsoluteRotation;
		Vector3 mPosition;
		Vector3 mAbsolutePosition;
		Matrix4x4 mModelMatrix;
		type_mask mTypeMask;

	private:
		SceneNode* mParent;
		ChildNodes mChildren;
		SceneGroup* mSceneGroup;
		LinkedList<Component, &Component::ComponentLink> mComponents;
	};
	
	//
	// Script integration
	//

	extern int SceneNode_Factory(lua_State* L);
	extern int SceneNode_GetID(lua_State* L);
	extern int SceneNode_AddComponent(lua_State* L);
	extern int SceneNode_RemoveComponent(lua_State* L);
	extern int SceneNode_GetPosition(lua_State* L);
	extern int SceneNode_SetPosition(lua_State* L);
	static luaL_Reg SceneNode_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneNode_Factory },
		{ LUA_TOSTRING, SceneNode_GetID },
		{ "GetID", SceneNode_GetID },
		{ "AddComponent", SceneNode_AddComponent },
		{ "RemoveComponent", SceneNode_RemoveComponent },
		{ "GetPosition", SceneNode_GetPosition },
		{ "SetPosition", SceneNode_SetPosition },
		{ NULL, NULL }
	};
}
