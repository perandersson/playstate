#pragma once

#include "../component.h"
#include "../../model/model.h"
#include "../../processor/renderable.h"
#include "../../resources/resource_manager.h"
#include "../../resources/resource_changed_listener.h"
#include "../../script/scriptable.h"
#include "../../processor/tickable.h"

namespace playstate
{
	class RenderDynamicModel : public Component, public Renderable, public Tickable, public Scriptable, public IResourceChangedListener
	{
	public:
		//
		// Constructor
		//
		// @param model The model we want to render onto the screen
		RenderDynamicModel(Resource<Model> model);

		//
		// Destructor
		~RenderDynamicModel();

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();
		virtual void OnEvent(uint32 typeID, uint32 messageID);

	// IResourceChangedListener
	public:
		virtual void OnLoaded(ResourceObject* object);
		virtual void OnUnloading(ResourceObject* object);

	// IRenderable
	public:
		virtual void PreRender(const RenderState& state, RenderBlockResultSet* resultSet);

	// Tickable
	public:
		virtual void Tick();

	private:
		Resource<Model> mModel;
	};
	
	//
	// Script integration
	//

	extern int RenderDynamicModel_Factory(lua_State* L);
	static luaL_Reg RenderDynamicModel_Methods[] = {
		{ LUA_CONSTRUCTOR, RenderDynamicModel_Factory },
		{ NULL, NULL }
	};
}
