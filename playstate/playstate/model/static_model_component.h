#pragma once

#include "../component/component.h"
#include "../component/renderable.h"
#include "model.h"
#include "../resources/resource_manager.h"
#include "../resources/resource_changed_listener.h"

namespace playstate
{
	//
	// Component used for drawing a static (non-animated) model onto the screen.
	class StaticModelComponent : public Component, public IResourceChangedListener
	{
	public:
		StaticModelComponent(Resource<Model> model);
		virtual ~StaticModelComponent();

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

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

	extern int StaticModelComponent_Factory(lua_State* L);
	static luaL_Reg StaticModelComponent_Methods[] = {
		{ LUA_CONSTRUCTOR, StaticModelComponent_Factory },
		{ NULL, NULL }
	};
}
