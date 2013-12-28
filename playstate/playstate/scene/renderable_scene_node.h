#pragma once

#include "scene_node.h"
#include "../model/model.h"
#include "../resources/resource_manager.h"
#include "../resources/resource_changed_listener.h"

namespace playstate
{
	class RenderableSceneNode : public SceneNode, public IResourceChangedListener
	{
	public:
		RenderableSceneNode(Resource<Model> model);
		virtual ~RenderableSceneNode();

	// SceneNode
	protected:
		virtual void OnAttachedToSceneGroup();
		virtual void OnDetachingFromSceneGroup();

	// IRenderable
	public:
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet);
		
	// IResourceChangedListener
	public:
		virtual void OnLoaded(ResourceObject* object);
		virtual void OnUnloading(ResourceObject* object);

	private:
		Resource<Model> mModel;
	};
	
	//
	// Script integration
	//

	extern int RenderableSceneNode_Factory(lua_State* L);
	static luaL_Reg RenderableSceneNode_Methods[] = {
		{ LUA_CONSTRUCTOR, RenderableSceneNode_Factory },
		{ NULL, NULL }
	};
}
