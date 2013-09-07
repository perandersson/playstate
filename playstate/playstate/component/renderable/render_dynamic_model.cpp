#include "../../memory/memory.h"
#include "render_dynamic_model.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
using namespace playstate;

RenderDynamicModel::RenderDynamicModel(Resource<Model> model) 
	: Component(), Renderable(), Tickable(), Scriptable(), mModel(model)
{
}

RenderDynamicModel::~RenderDynamicModel()
{
}

void RenderDynamicModel::OnComponentAdded()
{
	mModel.AddListener(this);
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
	Renderable::Attach(GetNode()->GetGroup());
	Tickable::Attach(GetNode()->GetGroup());
}

void RenderDynamicModel::OnComponentRemoved()
{
	Tickable::Detach();
	Renderable::Detach();
	mModel.RemoveListener(this);
}

void RenderDynamicModel::OnEvent(uint32 typeID, uint32 messageID)
{
	// Events which decides if this item should perform any special animations (i.e. damage taken etc)
}

void RenderDynamicModel::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
}

void RenderDynamicModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
}

void RenderDynamicModel::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
	
}

void RenderDynamicModel::Tick()
{
	// Update animation - 
}

namespace playstate
{
	int RenderDynamicModel_Factory(lua_State* L)
	{
		if(lua_gettop(L) < 1) {
			luaM_printerror(L, "Expected: RenderDynamicModel.__init(ResourceData)");
			lua_pushnil(L);
			return 1;
		}
		
		ResourceData* resourceData = luaM_popresource(L);
		if(resourceData != NULL) {
			Resource<Model> model(resourceData);
			RenderDynamicModel* renderStaticModel = new RenderDynamicModel(model);
			luaM_pushobject(L, "RenderDynamicModel", renderStaticModel);
		} else {
			luaM_printerror(L, "Expected: RenderDynamicModel.__init(ResourceData)");
			lua_pushnil(L);
		}
		return 1;
	}
}
