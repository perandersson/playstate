#include "../memory/memory.h"
#include "renderable_scene_node.h"
#include "scene_group.h"
using namespace playstate;

RenderableSceneNode::RenderableSceneNode(Resource<Model> model)
	: SceneNode(), mModel(model)
{
}

RenderableSceneNode::~RenderableSceneNode()
{
}


void RenderableSceneNode::OnAttachedToSceneGroup()
{
	mModel.AddListener(this);
	SetBoundingBox(mModel->GetBoundingBox(), GetPosition(), GetScale());
	GetGroup()->AttachRenderable(this);
}

void RenderableSceneNode::OnDetachingFromSceneGroup()
{
	GetGroup()->DetachRenderable(this);
	mModel.RemoveListener(this);
}

void RenderableSceneNode::OnLoaded(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetPosition(), GetScale());
}

void RenderableSceneNode::OnUnloading(ResourceObject* object)
{
	SetBoundingBox(mModel->GetBoundingBox(), GetPosition(), GetScale());
}

void RenderableSceneNode::PreRender(const RenderState& state, RenderBlockResultSet* resultSet)
{
	uint32 size = mModel->GetNumMeshes();
	ModelMesh* meshes = mModel->GetMeshes();
	for(uint32 i = 0; i < size; ++i) {
		ModelMesh& mesh = meshes[i];
		RenderBlock* block = resultSet->Create(mesh.Id);
		block->ModelMatrix = GetModelMatrix();
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

int playstate::RenderableSceneNode_Factory(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: RenderableSceneNode.__init(Model)");
		lua_pushnil(L);
		return 1;
	}

	Resource<Model> model = luaM_popresource<Model>(L);
	if(model.IsNotNull()) {
		RenderableSceneNode* node = new RenderableSceneNode(model);
		luaM_pushobject(L, "RenderableSceneNode", node);
	} else {
		luaM_printerror(L, "Expected: RenderableSceneNode.__init(Model)");
		lua_pushnil(L);
	}
	return 1;
}
