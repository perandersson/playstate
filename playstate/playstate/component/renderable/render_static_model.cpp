#include "../../memory/memory.h"
#include "render_static_model.h"
#include "../../scene/scene_node.h"

using namespace playstate;

RenderStaticModel::RenderStaticModel(Resource<Model> model) 
	: Component(), Renderable(), Scriptable(), mModel(model)
{
}

RenderStaticModel::~RenderStaticModel()
{
}

void RenderStaticModel::OnComponentAdded()
{
	mModel.AddListener(this);
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
	Renderable::Attach(GetNode()->GetGroup());
}

void RenderStaticModel::OnComponentRemoved()
{
	Renderable::Detach();
	mModel.RemoveListener(this);
}

void RenderStaticModel::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
}

void RenderStaticModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
}

void RenderStaticModel::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
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
		}
		block->DiffuseColor = mesh.DiffuseColor;
	}
}

namespace playstate
{
	int RenderStaticModel_Factory(lua_State* L)
	{
		if(lua_gettop(L) < 1) {
			luaM_printerror(L, "Expected: RenderStaticModel.__init(Model)");
			lua_pushnil(L);
			return 1;
		}

		Resource<Model> model = luaM_popresource<Model>(L);
		if(model.IsNotNull()) {
			RenderStaticModel* renderStaticModel = new RenderStaticModel(model);
			luaM_pushobject(L, "RenderStaticModel", renderStaticModel);
		} else {
			luaM_printerror(L, "Expected: RenderStaticModel.__init(Model)");
			lua_pushnil(L);
		}
		return 1;
	}
}
