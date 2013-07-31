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
		const Vector3& GetPosition() const;
		const Vector3& GetAbsolutePosition() const;

		//
		// Sets the rotation of this node. 
		void SetRotation(const Vector3& rotation);
		const Vector3& GetRotation() const;
		const Vector3& GetAbsoluteRotation() const;

		//
		// @return This nodes model matrix
		const Matrix4x4& GetModelMatrix() const;

		//
		// @return
		type_mask GetTypeMask() const;

		//
		// @return The group where this node is located
		SceneGroup* GetGroup();

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
	extern int SceneNode_Translate(lua_State* L);
	extern int SceneNode_SetRotatation(lua_State* L);
	extern int SceneNode_AddChildNode(lua_State* L);
	extern int SceneNode_RemoveChildNode(lua_State* L);
	static luaL_Reg SceneNode_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneNode_Factory },
		{ LUA_TOSTRING, SceneNode_GetID },
		{ "GetID", SceneNode_GetID },
		{ "AddComponent", SceneNode_AddComponent },
		{ "RemoveComponent", SceneNode_RemoveComponent },
		{ "GetPosition", SceneNode_GetPosition },
		{ "SetPosition", SceneNode_SetPosition },
		{ "Translate", SceneNode_Translate },
		{ "SetRotation", SceneNode_SetRotatation },
		{ "AddChildNode", SceneNode_AddChildNode },
		{ "RemoveChildNode", SceneNode_RemoveChildNode },
		{ NULL, NULL }
	};
}
