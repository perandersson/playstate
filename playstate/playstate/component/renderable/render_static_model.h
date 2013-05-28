#pragma once

#include "../component.h"
#include "../../model/model.h"
#include "../../processor/renderable.h"
#include "../../scene/scene.h"
#include "../../resources/resource_manager.h"
#include "../../resources/resource_changed_listener.h"
#include "../../script/scriptable.h"

namespace playstate
{
	class RenderStaticModel : public Component, public Renderable, public IResourceChangedListener
	{
	public:
		//
		// Constructor
		//
		// @param model The model we want to render onto the screen
		RenderStaticModel(Resource<Model> model);

		//
		// Destructor
		~RenderStaticModel();

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

	// IResourceChangedListener
	public:
		virtual void OnLoaded(ResourceObject* object);
		virtual void OnUnloading(ResourceObject* object);

	// IRenderable
	public:
		virtual void CollectBuildingBlocks(RenderBlockResultSet& resultSet, RenderState& state);

	private:
		Resource<Model> mModel;
	};
	
	//
	// Script integration
	//

	extern int RenderStaticModel_Factory(lua_State* L);
	static luaL_Reg RenderStaticModel_Methods[] = {
		{ LUA_CONSTRUCTOR, RenderStaticModel_Factory },
		{ NULL, NULL }
	};
}
