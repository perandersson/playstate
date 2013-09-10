#include "../../memory/memory.h"
#include "render_dynamic_model.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
using namespace playstate;

RenderDynamicModel::RenderDynamicModel(Resource<DynamicModel> model) 
	: Component(), Renderable(), Tickable(), Scriptable(), mModel(model)
{
}

RenderDynamicModel::~RenderDynamicModel()
{
}

void RenderDynamicModel::OnComponentAdded()
{
	mModel.AddListener(this);
	SetBoundingBox(GetBoundingBox(), GetNode()->GetPosition());
	//SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
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
	SetBoundingBox(GetBoundingBox(), GetNode()->GetPosition());
	//SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
}

void RenderDynamicModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(GetBoundingBox(), GetNode()->GetPosition());
	//SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition());
}

void RenderDynamicModel::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
	uint32 size = mModel->GetNumMeshes();
	ModelMesh* meshes = mModel->GetMeshes();
	for(uint32 i = 0; i < size; ++i) {
		ModelMesh& mesh = meshes[i];
		RenderBlock* block = resultSet->Create(mesh.Id);
		block->ModelMatrix = GetNode()->GetModelMatrix();
		if(BIT_ISSET(state.Filter, RenderStateFilter::GEOMETRY)) {
			block->VertexBuffer = mesh.Vertices;
			block->IndexBuffer = mesh.Indices;
		}
		if(BIT_ISSET(state.Filter, RenderStateFilter::TEXTURES)) {
			block->DiffuseTexture = mesh.DiffuseTexture.Get();
			//block->AmbientTexture = mesh.AmbientTexture.Get();
			//block->SpecularTexture = mesh.SpecularTexture.Get();
			//block->SpecularHighlightTexture = mesh.SpecularHighlightTexture.Get();
			//block->AlphaTexture = mesh.AlphaTexture.Get();
			//block->BumpMapTexture = mesh.BumpMapTexture.Get();
			//block->DisplacementTexture = mesh.DisplacementTexture.Get();
		}
		block->DiffuseColor = mesh.DiffuseColor;
	}
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
			luaM_printerror(L, "Expected: RenderDynamicModel.__init(DynamicModel)");
			lua_pushnil(L);
			return 1;
		}
		
		Resource<DynamicModel> model = luaM_popresource<DynamicModel>(L);
		if(model.IsNotNull()) {
			RenderDynamicModel* renderStaticModel = new RenderDynamicModel(model);
			luaM_pushobject(L, "RenderDynamicModel", renderStaticModel);
		} else {
			luaM_printerror(L, "Expected: RenderDynamicModel.__init(DynamicModel)");
			lua_pushnil(L);
		}
		return 1;
	}
}
