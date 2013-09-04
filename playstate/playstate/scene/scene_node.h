#pragma once
#include "../script/scriptable.h"
#include "../linked_list.h"
#include "../component/component.h"
#include "../math/matrix4x4.h"

namespace playstate
{
	class SceneGroup;

	class SceneNode : public Scriptable
	{
	public:
		LinkedListLink<SceneNode> NodeLink;

	public:
		//
		// Default constructor
		SceneNode();

		//
		// Constructor
		// @param typeMask The scene node type mask
		SceneNode(type_mask typeMask);

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
		// @return The relative position of this node instance
		inline const Vector3& GetPosition() const {
			return mPosition;
		}

		//
		// @return The absolute position of this node instance
		inline const Vector3& GetAbsolutePosition() const {
			return mAbsolutePosition;
		}

		//
		// Sets the rotation of this node. 
		void SetRotation(const Vector3& rotation);

		//
		// @return The relative rotation of this node
		inline const Vector3& GetRotation() const {
			return mRotation;
		}

		//
		// @return The absolute rotation of this node
		inline const Vector3& GetAbsoluteRotation() const {
			return mAbsoluteRotation;
		}

		//
		// @return This nodes model matrix
		inline const Matrix4x4& GetModelMatrix() const {
			return mModelMatrix;
		}

		//
		// @return
		inline type_mask GetTypeMask() const {
			return mTypeMask;
		}

		//
		// @return The group where this node is located
		inline SceneGroup* GetGroup() {
			return mSceneGroup;
		}

		//
		// Remove this item from the scene. 
		// This will also delete this object and clean-up all related resources (child nodes for example).
		void RemoveFromScene();
		
		//
		// Add a node
		void AddNode(SceneNode* node);

		//
		// 
		void RemoveNode(SceneNode* node);

		//
		// Notifies this node that it's been attached to the supplied group
		// 
		// This method might be called while it's already attached to a group. This
		//	means that this node will be moved from one group to the next.
		//
		// @param group
		void NodeAttachedToSceneGroup(SceneGroup* group);

		//
		// Notifies this node that it's being detached from the supplied group
		void DetachingNodeFromSceneGroup(SceneGroup* group);

		//
		// Checks if this node is being attached to a scene group
		//
		// @return
		inline bool IsAttachedToSceneGroup() const {
			return mSceneGroup != NULL;
		}
		
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
		SceneGroup* mSceneGroup;
		LinkedList<Component> mComponents;
		LinkedList<SceneNode> mChildren;
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
	extern int SceneNode_AddNode(lua_State* L);
	extern int SceneNode_RemoveNode(lua_State* L);
	extern int SceneNode_FireEvent(lua_State* L);
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
		{ "AddNode", SceneNode_AddNode },
		{ "RemoveNode", SceneNode_RemoveNode },
		{ "FireEvent", SceneNode_FireEvent },
		{ NULL, NULL }
	};
}
