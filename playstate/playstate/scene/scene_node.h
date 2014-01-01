#pragma once
#include "../script/scriptable.h"
#include "../component/renderable.h"
#include "../linked_list.h"
#include "../component/component.h"
#include "../math/matrix4x4.h"
#include "../collision/aabb.h"
#include "../script/luam.h"

namespace playstate
{
	class SceneGroup;
	class ISpatialTree;
	class IRenderProcessor;

	//
	// Base class for nodes inside a scene. 
	class SceneNode : public Scriptable, public IRenderable
	{
		friend class SceneGroup;

	public:
		LinkedListLink<SceneNode> NodeLink;
		LinkedListLink<SceneNode> SpatialNodeLink;
		LinkedListLink<SceneNode> RenderableNodeLink;

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
		// Add the supplied component to this node
		//
		// @param component The component we want to add to this node.
		void AddComponent(Component* component);

		//
		// Remove the supplied component.
		//
		// @param component
		void RemoveComponent(Component* component);

		//
		// Retrieves the first component matching the the supplied type. If no component is found then return NULL.
		//
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
		// @return The absolute position of this node instance in relation to the attached group
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
		// Sets the scale of this node
		void SetScale(const Vector3& scale);

		//
		// @return The relative scale of this node
		inline const Vector3& GetScale() const {
			return mScale;
		}

		//
		// @return The absolute scale of this node
		inline const Vector3& GetAbsoluteScale() const {
			return mAbsoluteScale;
		}


		//
		// @return This nodes model matrix
		inline const Matrix4x4& GetModelMatrix() const {
			return mModelMatrix;
		}

		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox);
		
		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position);
		
		//
		// Sets the bounding box for this node.
		void SetBoundingBox(const AABB& boundingBox, const Vector3& position, const Vector3& scale);

		//
		// @return The bounding box
		inline const AABB& GetBoundingBox() const {
			return mBoundingBox;
		}
		
		//
		// Attaches this node to the supplied tree
		void AttachToTree(ISpatialTree* tree);
		
		// 
		// @return The spatial tree this node is attached to
		inline ISpatialTree* GetTree() {
			return mTree;
		}
		//
		// Detaches this node from the tree it's being contained inside.
		void DetachFromTree();
		
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
		void RemoveFromScene();
		
		//
		// Add a node
		void AddChild(SceneNode* node);

		//
		// 
		void RemoveChild(SceneNode* node);

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

	// Renderable
	public:
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet);
		virtual void Show();
		virtual void Hide();

	protected:
		//
		// Method called when a child is added to this node
		//
		// @param node
		virtual void OnChildAdded(SceneNode* node);

		//
		// Method invoked when a child is removed from this node
		//
		// @param node
		virtual void OnChildRemoved(SceneNode* node);

		void OnDetachedFromParent(SceneNode* parent);
		void OnAttachedToParent(SceneNode* parent);

	protected:
		//
		// Method invoked when this node is attached to a scene group
		virtual void OnAttachedToSceneGroup();

		//
		// Method invoked the moment before this node is removed from the
		// scene group it's attached to
		virtual void OnDetachingFromSceneGroup();
		
	protected:
		//
		// Updates the absolute position value based on the parents position
		void UpdatePosition(const Vector3& parentPosition);

		//
		// Updates the absolute rotation value based on the parents rotation
		void UpdateRotation(const Vector3& parentRotation);

		//
		// Updates the absolute scale value based on the parents scale
		void UpdateScale(const Vector3& parentScale);

		//
		// Update the model matrix value based on the position and rotation
		void UpdateModelMatrix();
		
	private:
		Vector3 mRotation;
		Vector3 mAbsoluteRotation;
		Vector3 mPosition;
		Vector3 mAbsolutePosition;
		Vector3 mScale;
		Vector3 mAbsoluteScale;
		Matrix4x4 mModelMatrix;
		AABB mBoundingBox;
		ISpatialTree* mTree;
		type_mask mTypeMask;

	private:
		SceneNode* mParent;
		SceneGroup* mSceneGroup;
		LinkedList<Component> mComponents;
		LinkedList<SceneNode> mChildren;

	private:
		IRenderProcessor* mRenderProcessor;
		bool mVisible;
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
	extern int SceneNode_SetScale(lua_State* L);
	extern int SceneNode_AddChild(lua_State* L);
	extern int SceneNode_RemoveChild(lua_State* L);
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
		{ "SetScale", SceneNode_SetScale },
		{ "AddChild", SceneNode_AddChild },
		{ "RemoveChild", SceneNode_RemoveChild },
		{ "FireEvent", SceneNode_FireEvent },
		{ NULL, NULL }
	};
}
