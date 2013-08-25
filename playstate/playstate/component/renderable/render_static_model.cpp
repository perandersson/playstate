#include "../../memory/memory.h"
#include "render_static_model.h"
#include "../../scene/scene_node.h"

using namespace playstate;

RenderStaticModel::RenderStaticModel(Resource<Model> model) 
	: Component(), Renderable(), mModel(model)
{
}

RenderStaticModel::~RenderStaticModel()
{
}

void RenderStaticModel::OnComponentAdded()
{
	mModel.AddListener(this);
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
	Renderable::Attach(GetNode()->GetGroup());
}

void RenderStaticModel::OnComponentRemoved()
{
	Renderable::Detach();
	mModel.RemoveListener(this);
}

void RenderStaticModel::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
}

void RenderStaticModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetNode()->GetAbsolutePosition());
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
		if(lua_gettop(L) < 1) {
			luaM_printerror(L, "Expected: RenderStaticModel.__init(ResourceData)");
			lua_pushnil(L);
			return 1;
		}
		
		ResourceData* resourceData = luaM_popresource(L);
		if(resourceData != NULL) {
			Resource<Model> model(resourceData);
			ScriptedRenderStaticModel* renderStaticModel = new ScriptedRenderStaticModel(model);
			luaM_pushobject(L, "RenderStaticModel", renderStaticModel);
		} else {
			luaM_printerror(L, "Expected: RenderStaticModel.__init(ResourceData)");
			lua_pushnil(L);
		}
		return 1;
	}
}
