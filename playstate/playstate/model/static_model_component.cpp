#include "../memory/memory.h"
#include "static_model_component.h"
using namespace playstate;

StaticModelComponent::StaticModelComponent(Resource<Model> model)
	: Component(), mModel(model)
{
}

StaticModelComponent::~StaticModelComponent()
{
}

void StaticModelComponent::OnComponentAdded()
{
	mModel.AddListener(this);
	GetNode()->SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
	EnableFeature(Component::Features::RENDERABLE);
}

void StaticModelComponent::OnComponentRemoved()
{
	mModel.RemoveListener(this);
}

void StaticModelComponent::OnLoaded(ResourceObject* object)
{
	GetNode()->SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
}

void StaticModelComponent::OnUnloading(ResourceObject* object)
{
	GetNode()->SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetPosition(), GetNode()->GetScale());
}

void StaticModelComponent::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
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

int playstate::StaticModelComponent_Factory(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: StaticModelComponent.__init(Model)");
		lua_pushnil(L);
		return 1;
	}

	Resource<Model> model = luaM_popresource<Model>(L);
	if(model.IsNotNull()) {
		StaticModelComponent* node = new StaticModelComponent(model);
		luaM_pushobject(L, "StaticModelComponent", node);
	} else {
		luaM_printerror(L, "Expected: StaticModelComponent.__init(Model)");
		lua_pushnil(L);
	}
	return 1;
}
