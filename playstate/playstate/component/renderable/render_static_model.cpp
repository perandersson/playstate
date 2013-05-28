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
	Renderable::Attach(Owner->Group);
	SetBoundingBox(mModel->BoundingBox, Owner->AbsolutePosition);
}

void RenderStaticModel::OnComponentRemoved()
{
	Renderable::Detach();
}

void RenderStaticModel::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->BoundingBox, Owner->AbsolutePosition);
}

void RenderStaticModel::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->BoundingBox, Owner->AbsolutePosition);
}

void RenderStaticModel::CollectBuildingBlocks(RenderBlockResultSet& resultSet, RenderState& state)
{
	const Model* model = mModel.Get();
	uint32 size = model->Size;
	for(uint32 i = 0; i < size; ++i) {
		const ModelMesh& mesh = model->Meshes[i];
		RenderBlock& block = resultSet.CreateAndGet(mesh.Id);
		block.ModelMatrix = Owner->ModelMatrix;
		if(BIT_ISSET(state.Filter, RenderStateFilter::GEOMETRY)) {
			block.VertexBuffer = mesh.Vertices;
			block.IndexBuffer = mesh.Indices;
		}
		if(BIT_ISSET(state.Filter, RenderStateFilter::DIFFUSE_TEXTURE)) {
			block.DiffuseTexture = mesh.DiffuseTexture.Get();
		}
		block.DiffuseColor = mesh.DiffuseColor;
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
