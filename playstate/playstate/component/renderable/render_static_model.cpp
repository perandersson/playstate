#include "../../memory/memory.h"
#include "render_static_model.h"
#include "../../scene/scene_node.h"

using namespace playstate;

RenderStaticModel::RenderStaticModel(Resource<Model> model) 
	: Component(), Renderable(), mModel(model)
{
	mModel.AddListener(this);
}

RenderStaticModel::~RenderStaticModel()
{
	mModel.RemoveListener(this);
}

void RenderStaticModel::OnComponentAdded()
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
	Renderable::Attach(GetNode()->GetGroup());
}

void RenderStaticModel::OnComponentRemoved()
{
	Renderable::Detach();
}

void RenderStaticModel::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
}

void RenderStaticModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
}

void RenderStaticModel::CollectBuildingBlocks(RenderBlockResultSet& resultSet, const RenderState& state)
{
	uint32 size = mModel->GetNumMeshes();
	ModelMesh* meshes = mModel->GetMeshes();
	for(uint32 i = 0; i < size; ++i) {
		ModelMesh& mesh = meshes[i];
		RenderBlock* block = resultSet.Create(mesh.Id);
		block->ModelMatrix = GetNode()->GetModelMatrix();
		if(BIT_ISSET(state.Filter, RenderStateFilter::GEOMETRY)) {
			block->VertexBuffer = mesh.Vertices;
			block->IndexBuffer = mesh.Indices;
		}
		if(BIT_ISSET(state.Filter, RenderStateFilter::TEXTURES)) {
			block->DiffuseTexture = mesh.DiffuseTexture.Get();
			block->AmbientTexture = mesh.AmbientTexture.Get();
			block->SpecularTexture = mesh.SpecularTexture.Get();
			block->SpecularHighlightTexture = mesh.SpecularHighlightTexture.Get();
			block->AlphaTexture = mesh.AlphaTexture.Get();
			block->BumpMapTexture = mesh.BumpMapTexture.Get();
			block->DisplacementTexture = mesh.DisplacementTexture.Get();
		}
		block->DiffuseColor = mesh.DiffuseColor;
	}
}

namespace playstate
{
	class ScriptedRenderStaticModel : public RenderStaticModel, public Scriptable
	{
	public:
		ScriptedRenderStaticModel(Resource<Model> model)
			: RenderStaticModel(model)
		{}

		virtual ~ScriptedRenderStaticModel()
		{
		}
	};

	int RenderStaticModel_Factory(lua_State* L)
	{
		ResourceData* resourceData = luaM_popobject<ResourceData>(L);
		Resource<Model> model(resourceData);
		if(resourceData != NULL) {
			ScriptedRenderStaticModel* renderStaticModel = new ScriptedRenderStaticModel(model);
			luaM_pushobject(L, "RenderStaticModel", renderStaticModel);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}
}
