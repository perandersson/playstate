#include "../../memory/memory.h"
#include "linked_list_render_processor.h"
#include "../../scene/scene_node.h"
using namespace playstate;

LinkedListRenderProcessor::LinkedListRenderProcessor() 
	: Scriptable(), mNodes(offsetof(SceneNode, RenderableNodeLink))
{
}

LinkedListRenderProcessor::~LinkedListRenderProcessor()
{
	mNodes.DeleteAll();
}

void LinkedListRenderProcessor::AttachRenderable(SceneNode* node)
{
	assert_not_null(node);
	mNodes.AddLast(node);
}

void LinkedListRenderProcessor::DetachRenderable(SceneNode* node)
{
	assert_not_null(node);
	mNodes.Remove(node);
}

bool LinkedListRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	RenderState state;
	state.Camera = query.Camera;
	state.Filter = query.Filter;

	const Frustum& frustum = query.Camera->GetViewFrustum();
	const uint32 numResults = target->GetNumResults();
	SceneNode* node = mNodes.First();
	while(node != NULL) {
		SceneNode* next = node->RenderableNodeLink.Tail;
		AABB::CollisionResult result = frustum.IsColliding(node->GetBoundingBox());
		if(result != AABB::OUTSIDE) {
			node->PreRender(state, target);
		}
		node = next;
	}

	return target->GetNumResults() > numResults;
}

int playstate::LinkedListRenderProcessor_Factory(lua_State* L)
{
	LinkedListRenderProcessor* processor = new LinkedListRenderProcessor();
	luaM_pushobject(L, "LinkedListRenderProcessor", processor);
	return 1;
}
